module;

#include <cstdint>
#include "sam.h"
#include "samv71q21b.h"
#include <component/spi.h>

module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;
import Platform.SamV71_clock;
import Platform.SamV71_IO_pin;

extern "C" {
void SPI0_Handler() {
    if (auto *driver = SamV71::SamV71_SPI::get_SPI0()) {
        driver->on_interrupt();
    } else {
        NVIC_DisableIRQ(SPI0_IRQn);
    }
}
}

namespace SamV71 {
    SamV71_SPI *SamV71_SPI::optional_SPI0_driver{};

    namespace {
        struct Definition {
            spi_registers_t *the_base;
            IRQn_Type the_irq_number;
        };
    }

    static Definition Definition_SPI0{.the_base = SPI0_REGS, .the_irq_number = SPI0_IRQn};

    static void *get_definition(const uint8_t id) { return id == 0 ? static_cast<void *>(&Definition_SPI0) : nullptr; }
}

namespace SamV71 {
    SamV71_SPI::SamV71_SPI(const uint8_t id, Abstract::Abstract_IO_pin& chip_select)
        : the_id(id), use_chip_select(chip_select), optional_definition(get_definition(id)) {
        if (the_id == 0) {
            optional_SPI0_driver = this;
        }
    }

    void SamV71_SPI::initialize() {
        SamV71_clock::enable_peripheral_clock(SPI0_INSTANCE_ID);
        disable_SPI();
        setup_SPI_registers(SamV71_clock::get_frequency() / SPI_bitrate);
        if (the_id == 0) {
            optional_SPI0_driver = this;
        }
        enable_SPI();
    }

    void SamV71_SPI::on_interrupt() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            uint32_t interrupt_mask = base->SPI_IMR;
            if ((interrupt_mask & SPI_IMR_TDRE_Msk) && (base->SPI_SR & SPI_SR_TDRE_Msk)) {
                uint8_t data;
                if (on_tx_ready(&data)) {
                    base->SPI_TDR = SPI_TDR_TD(data);
                } else {
                    base->SPI_IDR = SPI_IDR_TDRE(1);
                }
            }
            if ((interrupt_mask & SPI_IMR_RDRF_Msk) && (base->SPI_SR & SPI_SR_RDRF_Msk)) {
                uint8_t data = SPI_RDR_RD_Msk & base->SPI_RDR;
                if (on_rx_ready(data)) {
                } else {
                    base->SPI_IDR = SPI_IDR_RDRF(1);
                }
            }
            is_transaction_complete();
        }
    }

    bool SamV71_SPI::on_rx_ready(const uint8_t data) const {
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

    bool SamV71_SPI::on_tx_ready(uint8_t *data) const {
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

    bool SamV71_SPI::is_transaction_complete() {
        if (optional_request) {
            auto size = optional_request->get_transfer_size();
            auto count = optional_request->get_transfer_count();
            if (count >= size) {
                unselect_chip();
                disable_SPI();
                end_transaction();
                set_ready();
                execute_pending_transaction();
                return true;
            }
        }
        return false;
    }

    void SamV71_SPI::end_transaction() const {
        if (optional_request) {
            auto *semaphore = optional_request->get_semaphore();
            if (semaphore) {
                semaphore->give();
            }
        }
    }

    void SamV71_SPI::execute_pending_transaction() {
        if (is_ready()) {
            Abstract::Abstract_request *request{};
            if (the_queue.receive(&request)) {
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
                execute_pending_transaction();
                semaphore->take();
                success = true;
            }
        }
        return success;
    }
} // SamV71
