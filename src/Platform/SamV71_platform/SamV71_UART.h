//
// Created by aeols on 2026-08-24.
//

#pragma once
#include "Abstract_UART.h"

namespace SamV71 {
    class SamV71_UART : public Abstract_UART {
    public:
        void initialize() override;

        bool has_input() override;

        bool for_each_input(Optional_user, Optional_func) override;

        bool is_ready() override;

        int print(const char *ptr, int len) override;

        int put(uint8_t c) override;

        bool get(uint8_t *p_data) override;

        static void USART1_ISR_RX_Handler();

        static void USART1_ISR_TX_Handler();

    private:
        static void USART1_RX_INT_ENABLE();

        static void USART1_RX_INT_DISABLE();

        static void USART1_TX_INT_ENABLE();

        static void USART1_TX_INT_DISABLE();
    };
} // SamV71
