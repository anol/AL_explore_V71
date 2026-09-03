//
// Created by aeols on 2026-08-24.
//

#pragma once
#include "Abstract_UART.h"

namespace SamV71
{
    class SamV71_USART1 : public Abstract_UART
    {
    public:
        static SamV71_USART1* optional_one_and_only_UART;
        static uint32_t the_USART1_IRQ_count;
        static uint32_t the_USART1_IRQ_status;
        static uint32_t the_USART1_TX_count;
        static uint32_t the_USART1_RX_count;

    private:
        enum { RX_buffer_size = 128, TX_buffer_size = 128 };

        Ringbuffer<uint8_t, RX_buffer_size> the_RX_queue{};
        Ringbuffer<uint8_t, TX_buffer_size> the_TX_queue{};

    public:
        SamV71_USART1() = default;

        void initialize() override;

        bool is_ready() override { return true; }

        bool has_input() override { return the_RX_queue.get_fill() > 0; }

        bool put(const uint8_t data) override { return the_TX_queue.put(data); }

        bool get(uint8_t* data) override { return the_RX_queue.get(data); }

        bool for_each_input(Optional_user user, Optional_func func) override
        {
            return the_RX_queue.for_each(user, func);
        }

        int print(const char* data, int len) override;

        void on_receiver_interrupt();

        void on_transmitter_interrupt();

    private:
        static void enable_receiver_interrupt();

        static void disable_receiver_interrupt();

        static void enable_transmitter_interrupt();

        static void disable_transmitter_interrupt();
    };
} // SamV71
