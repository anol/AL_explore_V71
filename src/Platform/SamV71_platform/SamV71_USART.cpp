//
// Created by aeols on 2026-08-24.
//

module;
#include <cstdint>
#include "FreeRTOS.h"
#include "sam.h"

module Platform.SamV71_USART;
import Type.Abstract_UART;
import Type.Misc_type;
import Platform.SamV71_clock;

namespace SamV71_USART_definition {
    SamV71::SamV71_USART *optional_USART1{};

    namespace {
        struct Definition {
            usart_registers_t *the_base;
            IRQn_Type the_irq_number;
            uint32_t the_instance;
        };
    }

    static Definition Definition_USART1{.the_base = USART1_REGS, .the_irq_number = USART1_IRQn, .the_instance = USART1_INSTANCE_ID};

    static void *get_definition(const uint8_t id) { return id == 1 ? static_cast<void *>(&Definition_USART1) : nullptr; }
}

extern "C" {
void ISR_USART1() {
    using namespace SamV71_USART_definition;
    if (optional_USART1) {
        optional_USART1->ISR();
    } else {
        NVIC_DisableIRQ(USART1_IRQn);
    }
}
}

namespace SamV71 {
    static uint32_t the_USART_IRQ_count{};
    static uint32_t the_USART_TX_count{};
    static uint32_t the_USART_TX_overflow_count{};
    static uint32_t the_USART_RX_count{};
    static uint32_t the_USART_RX_overflow_count{};
}

namespace SamV71 {
    using namespace SamV71_USART_definition;

    SamV71_USART::SamV71_USART(const uint8_t id)
        : the_id(id), optional_definition(get_definition(id)) {
        configASSERT(optional_definition != nullptr);
    }

    void SamV71_USART::initialize() {
        init_USART();
        init_interrupt();
    }

    void SamV71_USART::init_USART() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            auto instance = static_cast<Definition *>(optional_definition)->the_instance;
            SamV71_clock::enable_peripheral_clock(instance);
            base->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk); // Reset UART
            base->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);                            // Enable UART
            base->US_MR =                                                                           // Set UART mode
                    US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | (0 <<
                        US_MR_USART_OVER_Pos);
            base->US_BRGR = US_BRGR_CD(81); // Set bitrate
            if (the_id == 1) {
                optional_USART1 = this;
            }
        }
    }

    void SamV71_USART::init_interrupt() {
        if (optional_definition) {
            auto irq_number = static_cast<Definition *>(optional_definition)->the_irq_number;
            NVIC_DisableIRQ(irq_number);
            NVIC_SetPriority(irq_number, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
            NVIC_ClearPendingIRQ(irq_number);
            NVIC_EnableIRQ(irq_number);
            enable_receiver_interrupt();
        }
    }

    void SamV71_USART::ISR() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            the_USART_IRQ_count = the_USART_IRQ_count + 1;
            const auto status = base->US_CSR;
            const uint32_t error_status =
                    status & (US_CSR_USART_OVRE_Msk | US_CSR_USART_FRAME_Msk | US_CSR_USART_PARE_Msk);
            if (error_status) {
                base->US_IDR = // Disable Overrun, Parity and Framing error interrupts
                        (US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);
            }
            bool context_switch{};
            if (status & US_CSR_USART_RXRDY_Msk) {
                ISR_RX_ready(context_switch);
            }
            if (status & (US_CSR_USART_TXRDY_Msk | US_CSR_USART_TXEMPTY_Msk)) {
                ISR_TX_ready(context_switch);
            }
            portYIELD_FROM_ISR(context_switch ? pdTRUE : pdFALSE);
        }
    }

    void SamV71_USART::ISR_RX_ready(bool &context_switch) {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            while (base->US_CSR & US_CSR_USART_RXRDY_Msk) {
                const auto data = static_cast<uint8_t>(base->US_RHR & 0xFF);
                if (the_RX_queue.ISR_send(data, context_switch)) {
                    the_USART_RX_count = the_USART_RX_count + 1;
                } else {
                    the_USART_RX_overflow_count = the_USART_RX_overflow_count + 1;
                }
            }
        }
    }

    void SamV71_USART::ISR_TX_ready(bool &context_switch) {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            uint8_t data;
            while (base->US_CSR & (US_CSR_USART_TXRDY_Msk | US_CSR_USART_TXEMPTY_Msk)) {
                if (the_TX_queue.ISR_receive(&data, context_switch)) {
                    base->US_THR = data;
                    the_USART_TX_count = the_USART_TX_count + 1;
                } else {
                    disable_transmitter_interrupt();
                    break;
                }
            }
        }
    }

    void SamV71_USART::enable_receiver_interrupt() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            base->US_IER = US_IER_USART_RXRDY_Msk;
            //  | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk | US_IER_USART_OVRE_Msk;
        }
    }

    void SamV71_USART::disable_receiver_interrupt() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            base->US_IDR = US_IDR_USART_RXRDY_Msk;
            //  | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk;
        }
    }

    void SamV71_USART::enable_transmitter_interrupt() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            base->US_IER = US_IER_USART_TXEMPTY_Msk | US_IER_USART_TXRDY_Msk;
        }
    }

    void SamV71_USART::disable_transmitter_interrupt() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            base->US_IDR = US_IDR_USART_TXEMPTY_Msk | US_IDR_USART_TXRDY_Msk;
        }
    }

    bool SamV71_USART::for_each_input(Optional_user user, Optional_func func) {
        uint8_t data;
        while (the_RX_queue.receive(&data)) {
            if (func) {
                func(user, data);
            }
        }
        return true;
    }

    int SamV71_USART::print(const char *data, int len) {
        int written = 0;
        while (len-- > 0) {
            if (the_TX_queue.send(*data)) {
                ++data;
                ++written;
            } else {
                the_USART_TX_overflow_count = the_USART_TX_overflow_count + 1;
                break;
            }
        }
        enable_transmitter_interrupt();
        return written;
    }
} // SamV71
