//
// Created by aeols on 2026-09-10.
//

#pragma once
#include "Abstract_service.h"
#include "Console_receive_task.h"
#include "Console_transmit_task.h"

namespace Console
{
    class Console_service : public Abstract::Abstract_service
    {
        Console_receive_task the_receiver{};
        Console_transmit_task the_transmitter{};

    public:
        Console_service() = default;
    };
} // Console
