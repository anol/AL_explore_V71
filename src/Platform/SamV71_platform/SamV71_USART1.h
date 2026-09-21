//
// Created by aeols on 2026-08-24.
//

#pragma once
#include "Misc_type.h"

import Type.Abstract_UART;

extern "C" {
#include "FreeRTOS.h"
#include "queue.h"
}

namespace SamV71 {
    class SamV71_USART1 : public Abstract::Abstract_UART {
    public:
        static SamV71_USART1 *optional_one_and_only_UART;
        static uint32_t       the_USART1_IRQ_count;
        static uint32_t       the_USART1_IRQ_status;
        static uint32_t       the_USART1_TX_count;
        static uint32_t       the_USART1_TX_overflow_count;
        static uint32_t       the_USART1_RX_count;
        static uint32_t       the_USART1_RX_overflow_count;

    private:
        enum { RX_queue_size = 128, TX_queue_size = 4096 };

        StaticQueue_t the_RX_structure{};
        uint8_t       the_RX_storage[RX_queue_size]{};
        StaticQueue_t the_TX_structure{};
        uint8_t       the_TX_storage[TX_queue_size]{};
        QueueHandle_t optional_RX_queue{};
        QueueHandle_t optional_TX_queue{};

    public:
        SamV71_USART1();

        void initialize() override;

        bool is_ready() override { return true; }

        bool has_input() override { return uxQueueMessagesWaiting(optional_RX_queue) > 0; }

        Status_code put(const uint8_t data) override { return Status_code(xQueueSend(optional_TX_queue, &data, 0) == pdPASS); }

        Status_code get(uint8_t *data) override { return Status_code(xQueueReceive(optional_RX_queue, data, 100) == pdPASS); }

        bool for_each_input(Optional_user user, Optional_func func) override;

        int print(const char *data, int len) override;

        void on_receiver_interrupt(BaseType_t *pxHigherPriorityTaskWoken);

        void on_transmitter_interrupt(BaseType_t *pxHigherPriorityTaskWoken);

    private:
        void init_USART1();

        static void init_interrupt();

        static void enable_receiver_interrupt();

        static void disable_receiver_interrupt();

        static void enable_transmitter_interrupt();

        static void disable_transmitter_interrupt();
    };
} // SamV71
