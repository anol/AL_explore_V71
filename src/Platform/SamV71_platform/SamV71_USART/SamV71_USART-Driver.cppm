//
// Created by aeols on 2026-08-24.
//

module;

#include <cstdint>
#include <cstdio>

#include "samv71q21b.h"
#include "component/usart.h"

module Platform.SamV71_USART:Driver;
import :Config;

import Platform.FreeRTOS_queue;

namespace SamV71 {
    class SamV71_USART_driver {
        enum {
            RX_queue_size  = 128,
            TX_queue_size  = 4096,
            Clock_divider  = 81,
        };

        const SamV71_USART_config *optional_config;
        FreeRTOS::FreeRTOS_queue<uint8_t, RX_queue_size> the_RX_queue{};
        FreeRTOS::FreeRTOS_queue<uint8_t, TX_queue_size> the_TX_queue{};
        uint32_t the_IRQ_count{};
        uint32_t the_RX_count{};
        uint32_t the_RX_overflow_count{};
        uint32_t the_TX_count{};
        uint32_t the_TX_overflow_count{};

    public:
        explicit SamV71_USART_driver(const SamV71_USART_config *config) : optional_config(config) {
        }

        void print_diagnostics() const;

        void setup_USART_registers() const;

        void enable_USART() const;

        void disable_USART() const;

        void enable_receiver_interrupt() const;

        void disable_receiver_interrupt() const;

        void enable_transmitter_interrupt() const;

        void disable_transmitter_interrupt() const;

        bool send(const uint8_t data) { return the_TX_queue.send(data); }

        bool receive(uint8_t *data) { return the_RX_queue.receive(data); }

        int print(const char *data, int len);

        void ISR_count_IRQ() { the_IRQ_count = the_IRQ_count + 1; }

        void ISR_received(uint8_t data, uint32_t &context_switch);

        bool ISR_get_transmit(uint8_t &data, uint32_t &context_switch);
    };

    void SamV71_USART_driver::print_diagnostics() const {
        if (optional_config) {
            printf("USART%d: pri=%d, irq=%u, rx=%u (overflow %u), tx=%u (overflow %u)\r\n",
                   optional_config->the_ID_number, static_cast<int>(optional_config->the_IRQ_priority),
                   static_cast<unsigned>(the_IRQ_count),
                   static_cast<unsigned>(the_RX_count), static_cast<unsigned>(the_RX_overflow_count),
                   static_cast<unsigned>(the_TX_count), static_cast<unsigned>(the_TX_overflow_count));
        }
    }

    void SamV71_USART_driver::setup_USART_registers() const {
        if (optional_config) {
            auto *base = optional_config->the_base;
            base->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk); // Reset UART
            base->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);                            // Enable UART
            base->US_MR =                                                                           // Set UART mode
                    US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT |
                    (0 << US_MR_USART_OVER_Pos);
            base->US_BRGR = US_BRGR_CD(Clock_divider); // Set bitrate
            const auto irq_number = optional_config->the_IRQ_number;
            const auto priority = optional_config->the_IRQ_priority;
            NVIC_DisableIRQ(irq_number);
            NVIC_SetPriority(irq_number, priority);
        }
    }

    void SamV71_USART_driver::enable_USART() const {
        if (optional_config) {
            const auto irq_number = optional_config->the_IRQ_number;
            NVIC_ClearPendingIRQ(irq_number);
            NVIC_EnableIRQ(irq_number);
            enable_receiver_interrupt();
        }
    }

    void SamV71_USART_driver::disable_USART() const {
        if (optional_config) {
            const auto irq_number = optional_config->the_IRQ_number;
            NVIC_DisableIRQ(irq_number);
            disable_receiver_interrupt();
            disable_transmitter_interrupt();
            NVIC_ClearPendingIRQ(irq_number);
        }
    }

    void SamV71_USART_driver::enable_receiver_interrupt() const {
        if (optional_config) {
            auto *base = optional_config->the_base;
            base->US_IER = US_IER_USART_RXRDY_Msk;
            //  | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk | US_IER_USART_OVRE_Msk;
        }
    }

    void SamV71_USART_driver::disable_receiver_interrupt() const {
        if (optional_config) {
            auto *base = optional_config->the_base;
            base->US_IDR = US_IDR_USART_RXRDY_Msk;
            //  | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk;
        }
    }

    void SamV71_USART_driver::enable_transmitter_interrupt() const {
        if (optional_config) {
            auto *base = optional_config->the_base;
            base->US_IER = US_IER_USART_TXEMPTY_Msk | US_IER_USART_TXRDY_Msk;
        }
    }

    void SamV71_USART_driver::disable_transmitter_interrupt() const {
        if (optional_config) {
            auto *base = optional_config->the_base;
            base->US_IDR = US_IDR_USART_TXEMPTY_Msk | US_IDR_USART_TXRDY_Msk;
        }
    }

    int SamV71_USART_driver::print(const char *data, int len) {
        int written = 0;
        while (len-- > 0) {
            if (the_TX_queue.send(*data)) {
                ++data;
                ++written;
            } else {
                the_TX_overflow_count = the_TX_overflow_count + 1;
                break;
            }
        }
        enable_transmitter_interrupt();
        return written;
    }

    void SamV71_USART_driver::ISR_received(const uint8_t data, uint32_t &context_switch) {
        if (the_RX_queue.ISR_send(data, context_switch)) {
            the_RX_count = the_RX_count + 1;
        } else {
            the_RX_overflow_count = the_RX_overflow_count + 1;
        }
    }

    bool SamV71_USART_driver::ISR_get_transmit(uint8_t &data, uint32_t &context_switch) {
        if (the_TX_queue.ISR_receive(&data, context_switch)) {
            the_TX_count = the_TX_count + 1;
            return true;
        }
        return false;
    }
}
