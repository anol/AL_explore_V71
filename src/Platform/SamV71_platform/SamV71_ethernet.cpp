//
// See Ethernet_samv71_mac.h for design notes, scope, and the caveat that
// this driver has not been compiled or hardware-tested in this
// environment (no SAMV71 CMSIS pack/toolchain available here).
//

#include <cstring>

#include "SamV71_ethernet.h"

#include "nvic_helpers.h"
#include "core_cm7.h"

namespace SamV71_platform {
    namespace {
        constexpr uint32_t Mdio_idle_timeout_iterations = 100000;

        // GMAC's descriptor-ring registers/fields are 32-bit, matching the
        // SAMV71's 32-bit address space. Going through uintptr_t (rather
        // than casting a pointer straight to uint32_t) keeps this
        // portable/well-defined even on a 64-bit host, e.g. when
        // compiling this file for a host-side unit test.
        uint32_t to_reg_addr(const void *ptr) {
            return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ptr));
        }
    } // namespace

    SamV71_ethernet *SamV71_ethernet::Instance = nullptr;

    Ethernet_status SamV71_ethernet::initialize(const uint8_t mac_address[6]) {
        if (the_initialized_flag) {
            the_last_error = Ethernet_status::Already_initialized;
            return the_last_error;
        }
        if (mac_address == nullptr) {
            the_last_error = Ethernet_status::Invalid_parameter;
            return the_last_error;
        }
        if (Instance != nullptr) {
            // Only one GMAC peripheral exists on this part; refuse to
            // hijack an already-active instance's interrupt binding.
            the_last_error = Ethernet_status::Already_initialized;
            return the_last_error;
        }

        std::memcpy(the_mac_address, mac_address, 6);

        // 1. Enable the GMAC peripheral clock.
        PMC->PMC_PCER1 = (1u << (ID_GMAC - 32));

        // 2. Disable everything while we configure it.
        GMAC->GMAC_NCR = 0;
        GMAC->GMAC_IDR = 0xFFFFFFFFu;  // disable all interrupt sources
        (void) GMAC->GMAC_ISR;          // clear any pending/stale status
        GMAC->GMAC_RSR = GMAC->GMAC_RSR;  // write-1-to-clear
        GMAC->GMAC_TSR = GMAC->GMAC_TSR;  // write-1-to-clear

        // 3. Network configuration: MDC divider, 100BASE-TX full duplex
        //    (see configure_link() to change after autonegotiation), and
        //    allow frames up to 1536 bytes.
        GMAC->GMAC_NCFGR = (select_mdc_clock_divider(the_mck_hz) << Ncfgr_clk_field_pos)
                          | GMAC_NCFGR_SPD
                          | GMAC_NCFGR_FD
                          | GMAC_NCFGR_MAXFS;

        // 4. DMA configuration: one full max-size frame per RX buffer
        //    (DRBS = 1536 / 64), full-size TX packet buffer, INCR4 AHB
        //    bursts.
        GMAC->GMAC_DCFGR = ((Frame_buffer_size / 64u) << 16)  // DRBS field, Pos=16
                          | GMAC_DCFGR_TXPBMS
                          | GMAC_DCFGR_RXBMS_FULL
                          | GMAC_DCFGR_FBLDO_INCR4;

        // 5. RMII mode (vs. MII) -- most SAMV71 boards wire the PHY over
        //    RMII.
        GMAC->GMAC_UR &= ~GMAC_UR_RMII;

        // 6. MAC address (Specific Address register 1).
        GMAC->GMAC_SA[0].GMAC_SAB =
                static_cast<uint32_t>(the_mac_address[0]) |
                (static_cast<uint32_t>(the_mac_address[1]) << 8) |
                (static_cast<uint32_t>(the_mac_address[2]) << 16) |
                (static_cast<uint32_t>(the_mac_address[3]) << 24);
        GMAC->GMAC_SA[0].GMAC_SAT =
                static_cast<uint32_t>(the_mac_address[4]) |
                (static_cast<uint32_t>(the_mac_address[5]) << 8);

        // 7. Descriptor rings.
        init_descriptor_rings();
        clean_cache_for_write(the_rx_descriptors, sizeof(the_rx_descriptors));
        clean_cache_for_write(the_tx_descriptors, sizeof(the_tx_descriptors));
        GMAC->GMAC_RBQB = to_reg_addr(&the_rx_descriptors[0]);
        GMAC->GMAC_TBQB = to_reg_addr(&the_tx_descriptors[0]);
        the_rx_tail_index = 0;
        the_tx_head_index = 0;

        // 8. Enable the MDIO management port so we can talk to the PHY,
        //    then bring RX/TX up.
        GMAC->GMAC_NCR |= GMAC_NCR_MPE;
        GMAC->GMAC_NCR |= (GMAC_NCR_TXEN | GMAC_NCR_RXEN);

        // 9. Interrupts: receive-complete/used-bit-read to drive poll(),
        //    plus a small set of error conditions we surface via
        //    get_last_error().
        GMAC->GMAC_IER = GMAC_ISR_RCOMP | GMAC_ISR_RXUBR | GMAC_ISR_ROVR;

        Instance = this;
        NVIC_ClearPendingIRQ(GMAC_IRQn);
        NVIC_EnableIRQ(GMAC_IRQn);

        the_initialized_flag = true;
        the_last_error = Ethernet_status::Ok;
        return Ethernet_status::Ok;
    }

    Ethernet_status SamV71_ethernet::shutdown() {
        if (!the_initialized_flag) {
            return Ethernet_status::Ok;
        }
        NVIC_DisableIRQ(GMAC_IRQn);
        GMAC->GMAC_IDR = 0xFFFFFFFFu;
        GMAC->GMAC_NCR = 0;
        if (Instance == this) {
            Instance = nullptr;
        }
        the_receive_callback = nullptr;
        the_initialized_flag = false;
        return Ethernet_status::Ok;
    }

    bool SamV71_ethernet::is_initialized() const { return the_initialized_flag; }

    void SamV71_ethernet::init_descriptor_rings() {
        for (size_t i = 0; i < Num_rx_descriptors; ++i) {
            uint32_t addr = to_reg_addr(&the_rx_buffers[i][0]) & Rx_address_mask;
            if (i == Num_rx_descriptors - 1) {
                addr |= Rx_wrap_bit;
            }
            // Ownership bit clear: descriptor is available for GMAC to fill.
            the_rx_descriptors[i].address = addr;
            the_rx_descriptors[i].status = 0;
        }
        for (size_t i = 0; i < Num_tx_descriptors; ++i) {
            the_tx_descriptors[i].address = to_reg_addr(&the_tx_buffers[i][0]);
            uint32_t status = Tx_used_bit;  // available for software to fill
            if (i == Num_tx_descriptors - 1) {
                status |= Tx_wrap_bit;
            }
            the_tx_descriptors[i].status = status;
        }
    }

    uint32_t SamV71_ethernet::select_mdc_clock_divider(uint32_t mck_hz) const {
        // MDC must stay at or below ~2.5 MHz per IEEE 802.3 Clause 22. The
        // thresholds below follow the pattern used across Microchip's SAM
        // GMAC-family reference drivers (divide MCK down until it's under
        // the limit); double-check against the exact datasheet revision
        // for your part before shipping, as some SAM parts document
        // slightly different cutoffs.
        if (mck_hz <= 20000000u) return 0u;  // MCK/8
        if (mck_hz <= 40000000u) return 1u;  // MCK/16
        if (mck_hz <= 80000000u) return 2u;  // MCK/32
        if (mck_hz <= 120000000u) return 3u; // MCK/48
        if (mck_hz <= 160000000u) return 4u; // MCK/64
        return 5u;                           // MCK/96
    }

    bool SamV71_ethernet::wait_mdio_idle() const {
        for (uint32_t i = 0; i < Mdio_idle_timeout_iterations; ++i) {
            if (GMAC->GMAC_NSR & GMAC_NSR_IDLE) {
                return true;
            }
        }
        the_last_error = Ethernet_status::Timeout;
        return false;
    }

    uint16_t SamV71_ethernet::mdio_read(uint8_t phy_addr, uint8_t reg_addr) const {
        GMAC->GMAC_MAN = Mdio_clause22_bit | Mdio_turnaround | Mdio_op_read
                        | (static_cast<uint32_t>(phy_addr & 0x1Fu) << Mdio_phy_addr_shift)
                        | (static_cast<uint32_t>(reg_addr & 0x1Fu) << Mdio_reg_addr_shift);
        if (!wait_mdio_idle()) {
            return 0;
        }
        return static_cast<uint16_t>(GMAC->GMAC_MAN & Mdio_data_mask);
    }

    void SamV71_ethernet::mdio_write(uint8_t phy_addr, uint8_t reg_addr, uint16_t value) {
        GMAC->GMAC_MAN = Mdio_clause22_bit | Mdio_turnaround | Mdio_op_write
                        | (static_cast<uint32_t>(phy_addr & 0x1Fu) << Mdio_phy_addr_shift)
                        | (static_cast<uint32_t>(reg_addr & 0x1Fu) << Mdio_reg_addr_shift)
                        | (value & Mdio_data_mask);
        (void) wait_mdio_idle();
    }

    bool SamV71_ethernet::is_link_up() const {
        if (!the_initialized_flag) {
            return false;
        }
        // BMSR bit 2 (Link Status) latches low on a link failure; the
        // standard way to get the *current* status is to read it twice
        // and keep the second value.
        (void) mdio_read(the_phy_address, Phy_reg_bmsr);
        const uint16_t bmsr = mdio_read(the_phy_address, Phy_reg_bmsr);
        return (bmsr & Phy_bmsr_link_up_bit) != 0;
    }

    void SamV71_ethernet::get_mac_address(uint8_t mac_address_out[6]) const {
        if (mac_address_out == nullptr) {
            return;
        }
        std::memcpy(mac_address_out, the_mac_address, 6);
    }

    void SamV71_ethernet::configure_link(bool speed_100mbit, bool full_duplex) {
        uint32_t ncfgr = GMAC->GMAC_NCFGR;
        ncfgr = speed_100mbit ? (ncfgr | GMAC_NCFGR_SPD) : (ncfgr & ~GMAC_NCFGR_SPD);
        ncfgr = full_duplex ? (ncfgr | GMAC_NCFGR_FD) : (ncfgr & ~GMAC_NCFGR_FD);
        GMAC->GMAC_NCFGR = ncfgr;
    }

    Ethernet_status SamV71_ethernet::send_frame(const uint8_t *data, size_t length) {
        if (!the_initialized_flag) {
            the_last_error = Ethernet_status::Not_initialized;
            return the_last_error;
        }
        if (data == nullptr || length == 0 || length > (Frame_buffer_size - 4)) {
            // -4: caller must not include the FCS; GMAC generates and
            // appends it.
            the_last_error = Ethernet_status::Invalid_parameter;
            return the_last_error;
        }

        Gmac_descriptor &desc = the_tx_descriptors[the_tx_head_index];
        invalidate_cache_for_read(&desc, sizeof(desc));
        if ((desc.status & Tx_used_bit) == 0) {
            // Still owned by GMAC -- ring is full / GMAC hasn't caught up.
            the_last_error = Ethernet_status::Hardware_error;
            return the_last_error;
        }

        uint8_t *buffer = the_tx_buffers[the_tx_head_index];
        std::memcpy(buffer, data, length);
        clean_cache_for_write(buffer, length);

        uint32_t status = (static_cast<uint32_t>(length) & Tx_status_len_mask) | Tx_last_buffer_bit;
        if (the_tx_head_index == Num_tx_descriptors - 1) {
            status |= Tx_wrap_bit;
        }
        // USED bit (Tx_used_bit) intentionally left clear: writing
        // status last, with USED=0, is what hands this descriptor to the
        // GMAC DMA engine.
        desc.status = status;
        clean_cache_for_write(&desc, sizeof(desc));

        __DSB();
        GMAC->GMAC_NCR |= GMAC_NCR_TSTART;

        the_tx_head_index = (the_tx_head_index + 1) % Num_tx_descriptors;
        the_last_error = Ethernet_status::Ok;
        return Ethernet_status::Ok;
    }

    Ethernet_status SamV71_ethernet::set_receive_callback(Ethernet_frame_callback callback) {
        the_receive_callback = std::move(callback);
        return Ethernet_status::Ok;
    }

    void SamV71_ethernet::poll() {
        if (!the_initialized_flag) {
            return;
        }
        // the_rx_pending_flag is just an optimization; the ring walk
        // below is always correct on its own, so a missed/racy flag only
        // costs one extra (cheap) call, never correctness.
        the_rx_pending_flag.store(false, std::memory_order_relaxed);

        for (size_t processed = 0; processed < Num_rx_descriptors; ++processed) {
            Gmac_descriptor &desc = the_rx_descriptors[the_rx_tail_index];
            invalidate_cache_for_read(&desc, sizeof(desc));

            if ((desc.address & Rx_ownership_bit) == 0) {
                break;  // no more frames ready
            }

            const uint32_t status = desc.status;
            const size_t length = status & Rx_status_len_mask;
            const bool sof_eof = (status & Rx_status_sof_bit) && (status & Rx_status_eof_bit);

            if (sof_eof && length > 0 && length <= Frame_buffer_size && the_receive_callback) {
                uint8_t *buffer = the_rx_buffers[the_rx_tail_index];
                invalidate_cache_for_read(buffer, length);
                the_receive_callback(buffer, length);
            }
            // else: fragmented frame spanning descriptors (shouldn't
            // happen with our full-frame-sized buffers), zero-length, or
            // oversized status -- drop it silently rather than passing
            // bad data up.

            // Release the descriptor back to GMAC.
            desc.address &= ~Rx_ownership_bit;
            clean_cache_for_write(&desc, sizeof(desc));

            the_rx_tail_index = (the_rx_tail_index + 1) % Num_rx_descriptors;
        }
    }

    Ethernet_status SamV71_ethernet::get_last_error() const { return the_last_error; }

    void SamV71_ethernet::handle_interrupt() {
        const uint32_t isr = GMAC->GMAC_ISR;  // read-to-clear

        if (isr & (GMAC_ISR_RCOMP | GMAC_ISR_RXUBR)) {
            the_rx_pending_flag.store(true, std::memory_order_relaxed);
        }
        if (isr & GMAC_ISR_ROVR) {
            the_last_error = Ethernet_status::Hardware_error;
        }

        // RSR/TSR are separate, write-1-to-clear status registers.
        const uint32_t rsr = GMAC->GMAC_RSR;
        GMAC->GMAC_RSR = rsr;
        const uint32_t tsr = GMAC->GMAC_TSR;
        GMAC->GMAC_TSR = tsr;
    }

    void SamV71_ethernet::clean_cache_for_write(const void *addr, size_t length) {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1)
        SCB_CleanDCache_by_Addr(const_cast<uint32_t *>(reinterpret_cast<const uint32_t *>(addr)),
                                 static_cast<int32_t>(length));
#else
        (void) addr;
        (void) length;
#endif
    }

    void SamV71_ethernet::invalidate_cache_for_read(const void *addr, size_t length) {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1)
        SCB_InvalidateDCache_by_Addr(const_cast<uint32_t *>(reinterpret_cast<const uint32_t *>(addr)),
                                      static_cast<int32_t>(length));
#else
        (void) addr;
        (void) length;
#endif
    }
} // SamV71_platform

// ---------------------------------------------------------------------
// Interrupt vector. The SAMV71 has exactly one GMAC peripheral, so this
// free function forwards to whichever instance called initialize() last.
extern "C" void GMAC_Handler(void) {
    if (SamV71_platform::SamV71_ethernet *instance = SamV71_platform::SamV71_ethernet::access_instance_for_isr()) {
        instance->handle_interrupt();
    }
}
