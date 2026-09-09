//
// Created by aeols on 2026-08-24.
//

#pragma once
#include "Abstract_UART.h"

extern "C" {
#include "FreeRTOS.h"
#include "queue.h"
}

namespace SamV71
{
    class SamV71_USART1 : public Abstract_UART
    {
    public:
        static SamV71_USART1* optional_one_and_only_UART;
        static uint32_t the_USART1_IRQ_count;
        static uint32_t the_USART1_IRQ_status;
        static uint32_t the_USART1_TX_count;
        static uint32_t the_USART1_TX_overflow_count;
        static uint32_t the_USART1_RX_count;
        static uint32_t the_USART1_RX_overflow_count;

    private:
        enum { RX_buffer_size = 128, TX_buffer_size = 4096 };

        QueueHandle_t the_RX_queue{};
        QueueHandle_t the_TX_queue{};

    public:
        SamV71_USART1();

        void initialize() override;

        bool is_ready() override { return true; }

        bool has_input() override { return uxQueueMessagesWaiting(the_RX_queue) > 0; }

        bool put(const uint8_t data) override { return xQueueSend(the_TX_queue, &data, 0) == pdPASS; }

        bool get(uint8_t* data) override { return xQueueReceive(the_RX_queue, data, 100) == pdPASS; }

        bool for_each_input(Optional_user user, Optional_func func) override;

        int print(const char* data, int len) override;

        void on_receiver_interrupt(BaseType_t* pxHigherPriorityTaskWoken);

        void on_transmitter_interrupt(BaseType_t* pxHigherPriorityTaskWoken);

    private:
        void init_USART1();

        static void init_interrupt();

        static void enable_receiver_interrupt();

        static void disable_receiver_interrupt();

        static void enable_transmitter_interrupt();

        static void disable_transmitter_interrupt();
    };
} // SamV71
