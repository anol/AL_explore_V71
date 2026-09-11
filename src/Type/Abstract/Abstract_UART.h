//
// Created by anolsen on 23.08.2019.
//

#pragma once

#include "Misc_type.h"
#include "Status_code.h"

namespace Abstract
{
    class Abstract_UART
    {
    public:
        Abstract_UART() = default;

        virtual ~Abstract_UART() = default;

        virtual void initialize() = 0;

        virtual bool has_input() = 0;

        virtual bool for_each_input(Optional_user, Optional_func) = 0;

        virtual bool is_ready() = 0;

        virtual int print(const char* data, int len) = 0;

        [[nodiscard]] virtual Status_code put(uint8_t data) = 0;

        [[nodiscard]] virtual Status_code get(uint8_t* data) = 0;
    };
}
