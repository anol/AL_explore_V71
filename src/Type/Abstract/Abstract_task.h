//
// Created by aeols on 2026-09-09.
//

#pragma once
#include <cstdint>

namespace Abstract
{
    class Abstract_task
    {
    public:
        virtual ~Abstract_task() = default;
        virtual void initialize() = 0;
        virtual void delay_until(uint32_t millis) = 0;
    };
} // Abstract
