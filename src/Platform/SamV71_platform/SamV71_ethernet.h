#pragma once

#include "Abstract/Abstract_ethernet.h"

#include <atomic>
#include <cstddef>
#include <cstdint>

// Pulls in the SAMV71 CMSIS device header for your specific part (defines
// the GMAC/PMC register structs, ID_GMAC, GMAC_IRQn, NVIC helpers, and the
// SCB cache-maintenance intrinsics). Your project normally defines the
// part macro (e.g. __SAMV71Q21B__) on the compiler command line and
// provides this umbrella header via the CMSIS-SAMV71 device pack.
#include "samv71q21b_symbols.h"
#include "samv71q21b.h"

using namespace Abstract;

namespace SamV71_platform {
    // Concrete Ethernet_device backend for the SAMV71's on-chip 10/100
    // Ethernet MAC (GMAC) peripheral, operated over RMII with an external
    // PHY.
    //
    // This is bare-metal: it programs GMAC's descriptor-ring DMA engine
    // directly. Register field names/positions were cross-checked against
    // Microchip's public GMAC documentation and Microchip/Atmel's
    // open-source reference drivers -- see the project README for links.
    // It has NOT been compiled or hardware-tested: this development
    // environment has no SAMV71 CMSIS pack or toolchain. Before relying
    // on this in a product:
    //   1. Build it against your actual CMSIS-SAMV71 device pack and fix
    //      up any macro-name differences between packs/versions.
    //   2. Verify the MDC clock divider thresholds in
    //      select_mdc_clock_divider() against the exact datasheet
    //      revision for your part.
    //   3. Confirm your board's PHY address and that its Basic Mode
    //      Status Register (address 0x01) implements the standard IEEE
    //      802.3 link status bit.
    //   4. Test on real hardware, especially D-cache maintenance around
    //      descriptors and buffers, and TX/RX under load.
    //
    // Simplifications (kept intentionally, to keep this a readable
    // reference rather than a full production stack):
    //  - Single RX/TX queue (GMAC queue 0). The extra priority queues
    //    (1-4) exist in hardware and must be pointed at a dummy,
    //    permanently-"used" descriptor or disabled per your part's
    //    errata; that setup is omitted here.
    //  - Each RX descriptor's buffer is sized to hold one full max-size
    //    frame (GMAC_DCFGR.DRBS set for a 1536-byte buffer), so this
    //    driver does not implement multi-descriptor frame reassembly.
    //  - No autonegotiation-driven speed/duplex switching: NCFGR is
    //    programmed for 100BASE-TX full duplex in initialize(). Call
    //    configure_link() afterwards if your application needs to react
    //    to a PHY's resolved speed/duplex.
    //  - Exactly one GMAC peripheral instance is assumed, matching the
    //    SAMV71, so the interrupt handler is wired to a single static
    //    instance pointer.
    //
    // Thread/interrupt safety: send_frame() and poll() are meant to be
    // called only from normal (non-interrupt) application context, and
    // not concurrently with each other without external locking. The
    // GMAC interrupt handler only clears interrupt sources and records
    // error status; it never touches the descriptor ring, so it can't
    // race with poll().
    //
    // Sizing / lifetime: an instance owns its descriptor rings and DMA
    // buffers as data members (tens of kilobytes) so that no dynamic
    // allocation is required. Because those buffers are written by DMA,
    // this object must have a fixed address for its entire lifetime --
    // give it static storage duration (a file-scope global, or a
    // function-local static), never allocate it on the stack or move/copy
    // it.
    class SamV71_ethernet final : public Abstract::Abstract_ethernet {
        // ---- Ring sizes / buffer sizing ----
        static constexpr size_t Num_rx_descriptors = 16;
        static constexpr size_t Num_tx_descriptors = 8;
        // One full max-size frame per descriptor; must be a multiple of
        // 64 bytes (GMAC_DCFGR.DRBS unit) -- 1536 satisfies that (24 *
        // 64) and is also a 32-byte D-cache line multiple.
        static constexpr size_t Frame_buffer_size = 1536;

        struct Gmac_descriptor {
            uint32_t address;
            uint32_t status;
        };

        // ---- Descriptor bit fields ---------------------------------
        // (SAMV71 datasheet, GMAC chapter: "Receive Buffer Descriptor
        // Entry" / "Transmit Buffer Descriptor Entry"; positions
        // cross-checked against Microchip's ASF gmac.h.)
        static constexpr uint32_t Rx_ownership_bit  = (1u << 0);
        static constexpr uint32_t Rx_wrap_bit       = (1u << 1);
        static constexpr uint32_t Rx_address_mask   = 0xFFFFFFFCu;
        static constexpr uint32_t Rx_status_len_mask = 0xFFFu;  // length incl. FCS
        static constexpr uint32_t Rx_status_sof_bit = (1u << 14);
        static constexpr uint32_t Rx_status_eof_bit = (1u << 15);

        static constexpr uint32_t Tx_used_bit        = (1u << 31);
        static constexpr uint32_t Tx_wrap_bit        = (1u << 30);
        static constexpr uint32_t Tx_error_bit       = (1u << 29);  // retry limit exceeded
        static constexpr uint32_t Tx_underrun_bit    = (1u << 28);
        static constexpr uint32_t Tx_last_buffer_bit = (1u << 15);
        static constexpr uint32_t Tx_status_len_mask = 0x1FFFu;

        // ---- MDIO (GMAC_MAN) frame fields, IEEE 802.3 Clause 22 ----
        static constexpr uint32_t Mdio_clause22_bit  = (1u << 30);
        static constexpr uint32_t Mdio_turnaround    = (0x2u << 16);
        static constexpr uint32_t Mdio_op_write      = (0x1u << 28);
        static constexpr uint32_t Mdio_op_read       = (0x2u << 28);
        static constexpr uint32_t Mdio_phy_addr_shift = 23;
        static constexpr uint32_t Mdio_reg_addr_shift = 18;
        static constexpr uint32_t Mdio_data_mask     = 0xFFFFu;

        static constexpr uint8_t  Phy_reg_bmsr       = 0x01;  // Basic Mode Status Register
        static constexpr uint16_t Phy_bmsr_link_up_bit = (1u << 2);

        static constexpr uint32_t Ncfgr_clk_field_pos = 18;  // 3-bit MDC divider field

        void init_descriptor_rings();
        [[nodiscard]] uint32_t select_mdc_clock_divider(uint32_t mck_hz) const;
        [[nodiscard]] uint16_t mdio_read(uint8_t phy_addr, uint8_t reg_addr) const;
        void mdio_write(uint8_t phy_addr, uint8_t reg_addr, uint16_t value);
        [[nodiscard]] bool wait_mdio_idle() const;

        static void clean_cache_for_write(const void *addr, size_t length);
        static void invalidate_cache_for_read(const void *addr, size_t length);

        uint8_t the_phy_address;
        uint32_t the_mck_hz;
        bool the_initialized_flag{};
        uint8_t the_mac_address[6]{};

        Ethernet_frame_callback the_receive_callback;
        mutable Ethernet_status the_last_error{Ethernet_status::Ok};

        // CAUTION: each Gmac_descriptor is only 8 bytes, so four of them
        // share one 32-byte D-cache line. GMAC's descriptor ring format
        // requires them packed at that 8-byte stride (padding each one
        // out to its own cache line, the way this driver does for RX/TX
        // data buffers, would desynchronize hardware's ring walk), which
        // means the per-descriptor clean/invalidate calls in this driver
        // operate at cache-line granularity. In practice this is masked
        // by this driver's single-threaded, single-context access
        // pattern, but the robust fix used by production drivers is to
        // map this array through the MPU as a non-cacheable/Device
        // region instead of relying on manual clean/invalidate.
        alignas(32) Gmac_descriptor the_rx_descriptors[Num_rx_descriptors]{};
        alignas(32) Gmac_descriptor the_tx_descriptors[Num_tx_descriptors]{};
        alignas(32) uint8_t the_rx_buffers[Num_rx_descriptors][Frame_buffer_size]{};
        alignas(32) uint8_t the_tx_buffers[Num_tx_descriptors][Frame_buffer_size]{};

        size_t the_rx_tail_index{};  // next descriptor poll() will inspect
        size_t the_tx_head_index{};  // next descriptor send_frame() will fill

        // Set by handle_interrupt() on RCOMP/RXUBR, consumed by poll().
        // This is purely an optimization so poll() can skip the ring
        // scan when the interrupt hasn't fired since the last call;
        // poll()'s ring scan is itself always correct regardless of this
        // flag's state.
        std::atomic<bool> the_rx_pending_flag{false};

        static SamV71_ethernet *Instance;

    public:
        // phy_address: the MDIO address (0-31) strapped on your board's
        // PHY. mck_hz: the frequency (Hz) of the peripheral clock (MCK)
        // feeding GMAC at the time initialize() runs; used only to pick
        // a spec-compliant MDC clock divider for MDIO transactions.
        explicit SamV71_ethernet(uint8_t phy_address, uint32_t mck_hz)
            : the_phy_address(phy_address), the_mck_hz(mck_hz) {
        }

        ~SamV71_ethernet() override { shutdown(); }

        SamV71_ethernet(const SamV71_ethernet &) = delete;
        SamV71_ethernet &operator=(const SamV71_ethernet &) = delete;

        Ethernet_status initialize(const uint8_t mac_address[6]) override;
        Ethernet_status shutdown() override;
        [[nodiscard]] bool is_initialized() const override;

        [[nodiscard]] bool is_link_up() const override;
        void get_mac_address(uint8_t mac_address_out[6]) const override;

        [[nodiscard]] Ethernet_status send_frame(const uint8_t *data, size_t length) override;

        Ethernet_status set_receive_callback(Ethernet_frame_callback callback) override;
        void poll() override;

        [[nodiscard]] Ethernet_status get_last_error() const override;

        // Invoked from the free-function GMAC_Handler() ISR defined in
        // the .cpp. Not part of Ethernet_device; public only so that the
        // C-linkage handler can reach it. Do not call this yourself.
        void handle_interrupt();

        // Used only by the free-function GMAC_Handler() ISR to find the
        // active instance. Not part of Ethernet_device or the public API
        // contract.
        static SamV71_ethernet *access_instance_for_isr() { return Instance; }

        // Reprograms NCFGR's speed/duplex bits directly -- e.g. after
        // your application inspects a PHY status register for the
        // autonegotiated link speed/duplex. Safe to call any time after
        // initialize().
        void configure_link(bool speed_100mbit, bool full_duplex);
    };
} // SamV71_platform
