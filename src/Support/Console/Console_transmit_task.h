//
// Created by aeols on 2026-09-10.
//

#pragma once
#include "Abstract_task.h"

namespace Console
{
    class Console_transmit_task : public Abstract::Abstract_task
    {
    public:
        void initialize() override;
    };
} // Console
