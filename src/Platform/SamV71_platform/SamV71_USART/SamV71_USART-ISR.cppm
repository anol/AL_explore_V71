//
// Created by aeols on 2026-08-24.
//

module;

#include <cstdint>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "samv71q21b.h"
#include "component/usart.h"

module Platform.SamV71_USART:ISR;
import :Config;
import :Driver;

namespace SamV71 {
    class SamV71_USART_ISR {
    public:
        enum { Number_of_USART_peripherals = 3 };

        static SamV71_USART_ISR *optional_ISR[Number_of_USART_peripherals];

    private:
        static constexpr uint32_t Error_mask = US_CSR_USART_OVRE_Msk | US_CSR_USART_FRAME_Msk | US_CSR_USART_PARE_Msk;
        static constexpr uint32_t TX_ready_mask = US_CSR_USART_TXRDY_Msk | US_CSR_USART_TXEMPTY_Msk;

        const SamV71_USART_config *optional_config;
        SamV71_USART_driver &use_driver;

    public:
        explicit SamV71_USART_ISR(const SamV71_USART_config *config, SamV71_USART_driver &driver)
            : optional_config(config), use_driver(driver) {
            if (config && config->the_ID_number < Number_of_USART_peripherals) {
                optional_ISR[config->the_ID_number] = this;
            }
        }

        void ISR();

    private:
        void ISR_RX_ready(usart_registers_t *base, uint32_t &context_switch);

        void ISR_TX_ready(usart_registers_t *base, uint32_t &context_switch);
    };

    SamV71_USART_ISR *SamV71_USART_ISR::optional_ISR[Number_of_USART_peripherals]{};

    void SamV71_USART_ISR::ISR() {
        if (optional_config) {
            auto *base = optional_config->the_base;
            use_driver.ISR_count_IRQ();
            const auto status = base->US_CSR;
            if (status & Error_mask) {
                base->US_IDR = // Disable Overrun, Parity and Framing error interrupts
                        (US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);
            }
            uint32_t context_switch{};
            if (status & US_CSR_USART_RXRDY_Msk) {
                ISR_RX_ready(base, context_switch);
            }
            if (status & TX_ready_mask) {
                ISR_TX_ready(base, context_switch);
            }
            portYIELD_FROM_ISR(context_switch);
        }
    }

    void SamV71_USART_ISR::ISR_RX_ready(usart_registers_t *base, uint32_t &context_switch) {
        while (base->US_CSR & US_CSR_USART_RXRDY_Msk) {
            use_driver.ISR_received(static_cast<uint8_t>(base->US_RHR & 0xFF), context_switch);
        }
    }

    void SamV71_USART_ISR::ISR_TX_ready(usart_registers_t *base, uint32_t &context_switch) {
        uint8_t data;
        while (base->US_CSR & TX_ready_mask) {
            if (use_driver.ISR_get_transmit(data, context_switch)) {
                base->US_THR = data;
            } else {
                use_driver.disable_transmitter_interrupt();
                break;
            }
        }
    }
} // SamV71

extern "C" {
void ISR_USART1() {
    if (auto *handler = SamV71::SamV71_USART_ISR::optional_ISR[1]) {
        handler->ISR();
    } else {
        NVIC_DisableIRQ(USART1_IRQn);
        NVIC_ClearPendingIRQ(USART1_IRQn);
    }
}
}
