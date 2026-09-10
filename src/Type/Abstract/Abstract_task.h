//
// Created by aeols on 2026-09-09.
//

#pragma once

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace Abstract
{
    class Abstract_task
    {
    protected:
        TaskHandle_t optional_task{};

    public:
        virtual ~Abstract_task() = default;
        virtual void initialize() = 0;
    };
} // Abstract
