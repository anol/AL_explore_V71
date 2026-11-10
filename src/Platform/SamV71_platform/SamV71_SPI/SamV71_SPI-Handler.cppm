//
// Created by aeols on 2026-11-04.
//

module;

#include "FreeRTOS.h"
#include "portmacro.h"
#include "samv71q21b.h"
#include "component/spi.h"

module Platform.SamV71_SPI:Handler;
import :Define;
import :Driver;
import Type.Transfer_request;

namespace SamV71 {
    class SamV71_SPI_handler {
    public:
        enum { Number_of_SPI_peripherals = 1 };

        static SamV71_SPI_handler *optional_handler[Number_of_SPI_peripherals];

    private:
        const SamV71_SPI_define *optional_define;
        SamV71_SPI_driver &use_driver;
        Generic::SPI_transfer_request *optional_request{};

    public:
        explicit SamV71_SPI_handler(const SamV71_SPI_define *define, SamV71_SPI_driver &driver)
            : optional_define(define), use_driver(driver) {
            if (define->the_ID_number < Number_of_SPI_peripherals) {
                optional_handler[define->the_ID_number] = this;
            }
        }

        void ISR();

    private:
        [[nodiscard]] bool is_busy() const { return optional_request != nullptr; }

        [[nodiscard]] bool is_ready() const { return optional_request == nullptr; }

        void ISR_pending_transaction(uint32_t &context_switch);

        void ISR_transfer_complete(uint32_t &context_switch);

        void ISR_RX_ready(spi_registers_t *base);

        void send_next_byte(spi_registers_t *base);
    };

    SamV71_SPI_handler *SamV71_SPI_handler::optional_handler[Number_of_SPI_peripherals]{};

    /*
     * Lockstep transfer: exactly one byte is in flight at any time. The next byte is
     * written to TDR only after the previous one has been read from RDR, so RDR can
     * never be overrun no matter how late the ISR runs (the SPI host simply pauses SCK).
     * TDRE is only used as a one-shot "kick" to start an idle peripheral; RDRF drives
     * the transfer.
     */
    void SamV71_SPI_handler::ISR() {
        if (optional_define) {
            uint32_t context_switch{};
            auto *base = optional_define->the_base;
            const uint32_t interrupt_status = base->SPI_SR;
            base->SPI_IDR = SPI_IDR_TDRE(1);
            if (interrupt_status & SPI_SR_RDRF_Msk) {
                ISR_RX_ready(base);
            }
            if (is_busy() && optional_request->is_all_received()) {
                ISR_transfer_complete(context_switch);
            }
            if (is_ready()) {
                ISR_pending_transaction(context_switch);
            }
            portYIELD_FROM_ISR(context_switch);
        }
    }

    void SamV71_SPI_handler::ISR_RX_ready(spi_registers_t *base) {
        const uint8_t data = 0xFF & base->SPI_RDR;
        if (optional_request && optional_request->is_more_to_receive()) {
            if (auto *buffer = optional_request->get_receive_buffer()) {
                buffer[optional_request->get_receive_count()] = data;
            }
            optional_request->count_received();
            if (optional_request->is_more_to_send()) {
                send_next_byte(base);
            }
        }
    }

    void SamV71_SPI_handler::send_next_byte(spi_registers_t *base) {
        const auto *buffer = optional_request->get_transmit_buffer();
        const uint8_t data = buffer ? buffer[optional_request->get_send_count()] : 0xFF;
        optional_request->count_sent();
        base->SPI_TDR = SPI_TDR_TD(data);
    }

    void SamV71_SPI_handler::ISR_transfer_complete(uint32_t &context_switch) {
        const auto *request = optional_request;
        optional_request = nullptr;
        request->get_chip_select().set();
        request->get_semaphore().ISR_give(context_switch);
    }

    void SamV71_SPI_handler::ISR_pending_transaction(uint32_t &context_switch) {
        auto *base = optional_define->the_base;
        while (Abstract::Abstract_request *request = use_driver.ISR_get_request(context_switch)) {
            auto *spi_request = dynamic_cast<Generic::SPI_transfer_request *>(request);
            if (spi_request == nullptr) {
                continue; // not an SPI request: drop it rather than dereference null
            }
            if (!spi_request->is_more_to_send()) {
                // zero-length transfer: complete immediately
                spi_request->get_semaphore().ISR_give(context_switch);
                continue;
            }
            if (base->SPI_SR & SPI_SR_RDRF_Msk) {
                (void) base->SPI_RDR; // flush any stale byte
            }
            optional_request = spi_request;
            optional_request->get_chip_select().clear();
            base->SPI_IER = SPI_IER_RDRF(1);
            send_next_byte(base);
            return;
        }
        use_driver.disable_SPI();
    }
} // SamV71

extern "C" {
void ISR_SPI0() {
    if (auto *handler = SamV71::SamV71_SPI_handler::optional_handler[0]) {
        handler->ISR();
    } else {
        NVIC_DisableIRQ(IRQn::SPI0_IRQn);
        NVIC_ClearPendingIRQ(SPI0_IRQn);
    }
}
}
