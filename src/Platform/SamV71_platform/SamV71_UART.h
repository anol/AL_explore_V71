//
// Created by aeols on 2026-08-24.
//

#pragma once
#include "Abstract_UART.h"

namespace SamV71
{
    class SamV71_UART : public Abstract_UART
    {
    public:
        void initialize(unsigned long bitrate) override;
        bool has_input() override;
        bool for_each_input(Optional_user, Optional_func) override;
        bool is_ready() override;
        int print(const char* ptr, int len) override;
        int put(uint8_t c) override;
        bool get(uint8_t* p_data) override;
    };
} // SamV71
