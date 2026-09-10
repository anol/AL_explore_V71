//
// Created by aeols on 2026-09-02.
//

#pragma once

#include "Abstract_UART.h"

namespace Win11
{
    class Win11_UART : public Abstract_UART
    {
    public:
        void initialize() override
        {
        }

        bool has_input() override
        {
            return false;
        }

        bool for_each_input(Optional_user, Optional_func) override
        {
            return false;
        }

        bool is_ready() override
        {
            return false;
        }

        int print(const char* data, int len) override
        {
            return 0;
        }

        Status_code put(uint8_t data) override
        {
            return Status_code::Failure();
        }

        Status_code get(uint8_t* data) override
        {
            return Status_code::Failure();
        }
    };
} // Win11
