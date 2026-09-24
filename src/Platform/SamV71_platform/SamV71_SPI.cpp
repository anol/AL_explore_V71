module;

#include <cstdint>
#include "sam.h"
#include "samv71q21b.h"
#include "component/spi.h"

#include "FreeRTOS.h"

module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;
import Platform.SamV71_clock;
import Platform.SamV71_IO_pin;

namespace SamV71_SPI_definition {
    static SamV71::SamV71_SPI *optional_SPI0{};

    namespace {
        struct Definition {
            spi_registers_t *the_base;
            IRQn_Type the_irq_number;
        };
    }

    static Definition Definition_SPI0{.the_base = SPI0_REGS, .the_irq_number = SPI0_IRQn};

    static void *get_definition(const uint8_t id) { return id == 0 ? static_cast<void *>(&Definition_SPI0) : nullptr; }
}

extern "C" {
void ISR_SPI0() {
    if (auto *driver = SamV71_SPI_definition::optional_SPI0) {
        driver->ISR();
    } else {
        NVIC_DisableIRQ(SPI0_IRQn);
    }
}
}

namespace SamV71 {
    using namespace SamV71_SPI_definition;

    SamV71_SPI::SamV71_SPI(const uint8_t id, Abstract::Abstract_IO_pin &chip_select)
        : the_id(id), use_chip_select(chip_select), optional_definition(get_definition(id)) {
        configASSERT(optional_definition != nullptr);
        if (the_id == 0) {
            optional_SPI0 = this;
        }
    }

    void SamV71_SPI::initialize() {
        SamV71_clock::enable_peripheral_clock(SPI0_INSTANCE_ID);
        disable_SPI();
        setup_SPI_registers(SamV71_clock::get_frequency() / SPI_bitrate);
        if (the_id == 0) {
            optional_SPI0 = this;
        }
        enable_SPI();
    }

    void SamV71_SPI::ISR() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            uint32_t context_switch{};
            uint32_t interrupt_mask = base->SPI_IMR;
            if ((interrupt_mask & SPI_IMR_TDRE_Msk) && (base->SPI_SR & SPI_SR_TDRE_Msk)) {
                uint8_t data;
                if (ISR_TX_ready(&data, context_switch)) {
                    base->SPI_TDR = SPI_TDR_TD(data);
                } else {
                    base->SPI_IDR = SPI_IDR_TDRE(1);
                }
            }
            if ((interrupt_mask & SPI_IMR_RDRF_Msk) && (base->SPI_SR & SPI_SR_RDRF_Msk)) {
                uint8_t data = SPI_RDR_RD_Msk & base->SPI_RDR;
                if (ISR_RX_ready(data, context_switch)) {
                } else {
                    base->SPI_IDR = SPI_IDR_RDRF(1);
                }
            }
            ISR_check_progress(context_switch);
            portYIELD_FROM_ISR(context_switch);
        }
    }

    bool SamV71_SPI::ISR_RX_ready(const uint8_t data, uint32_t &context_switch) const {
        if (optional_request) {
            const auto size = optional_request->get_transfer_size();
            const auto count = optional_request->get_transfer_count();
            if (auto *buffer = optional_request->get_receive_buffer()) {
                const bool is_more = count < size;
                if (is_more) {
                    *(buffer + count) = data;
                }
                return is_more;
            }
        }
        return false;
    }

    bool SamV71_SPI::ISR_TX_ready(uint8_t *data, uint32_t &context_switch) const {
        if (optional_request && data) {
            const auto size = optional_request->get_transfer_size();
            const auto count = optional_request->get_transfer_count();
            if (auto *buffer = optional_request->get_transmit_buffer()) {
                const bool is_more = count < size;
                if (is_more) {
                    *data = *(buffer + count);
                }
                optional_request->increment_count();
                return is_more;
            }
        }
        if (data) {
            *data = 0u;
        }
        return false;
    }

    bool SamV71_SPI::ISR_check_progress(uint32_t &context_switch) {
        if (optional_request) {
            auto size = optional_request->get_transfer_size();
            auto count = optional_request->get_transfer_count();
            if (count >= size) {
                unselect_chip();
                disable_SPI();
                ISR_transfer_complete(context_switch);
                set_ready();
                ISR_pending_transaction(context_switch);
                return true;
            }
        }
        return false;
    }

    void SamV71_SPI::ISR_transfer_complete(uint32_t &context_switch) const {
        if (optional_request) {
            if (auto *semaphore = optional_request->get_semaphore()) {
                semaphore->ISR_give(context_switch);
            }
        }
    }

    void SamV71_SPI::pending_transaction() {
        if (is_ready()) {
            Abstract::Abstract_request *request{};
            if (the_queue.receive(&request)) {
                optional_request = static_cast<Generic::Transfer_request *>(request);
                select_chip();
                enable_SPI();
            }
        }
    }

    void SamV71_SPI::ISR_pending_transaction(uint32_t &context_switch) {
        if (is_ready()) {
            Abstract::Abstract_request *request{};
            if (the_queue.ISR_receive(&request, context_switch)) {
                optional_request = static_cast<Generic::Transfer_request *>(request);
                select_chip();
                enable_SPI();
            }
        }
    }

    void SamV71_SPI::setup_SPI_registers(const uint32_t bitrate_divider) const {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            base->SPI_CSR[0] =
                    SPI_CSR_SCBR(bitrate_divider) |
                    SPI_CSR_NCPHA(1) |  // Clock phase
                    SPI_CSR_DLYBS(0u) | // Delay before select
                    SPI_CSR_DLYBCT(0u); // Delay between consecutive transfers
            base->SPI_MR = SPI_MR_MSTR(1);
        }
    }

    void SamV71_SPI::enable_SPI() const {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            auto irq_number = static_cast<Definition *>(optional_definition)->the_irq_number;
            base->SPI_CR |= SPI_CR_SPIEN(1);
            NVIC_DisableIRQ(irq_number);
            base->SPI_IER = SPI_IER_RDRF(1) | SPI_IER_TDRE(1);
            NVIC_ClearPendingIRQ(irq_number);
            NVIC_EnableIRQ(irq_number);
        }
    }

    void SamV71_SPI::disable_SPI() const {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            auto irq_number = static_cast<Definition *>(optional_definition)->the_irq_number;
            NVIC_DisableIRQ(irq_number);
            base->SPI_IDR = (
                SPI_IDR_RDRF(1) |
                SPI_IDR_TDRE(1) |
                SPI_IDR_MODF(1) |
                SPI_IDR_OVRES(1) |
                SPI_IDR_NSSR(1) |
                SPI_IDR_TXEMPTY(1) |
                SPI_IDR_UNDES(1));
            NVIC_ClearPendingIRQ(irq_number);
            NVIC_EnableIRQ(irq_number);
        }
    }

    bool SamV71_SPI::transfer(Abstract::Abstract_request *request) {
        bool success{};
        auto *semaphore = request->get_semaphore();
        if (request && semaphore) {
            if (the_queue.send(request)) {
                pending_transaction();
                semaphore->take();
                success = true;
            }
        }
        return success;
    }
} // SamV71
