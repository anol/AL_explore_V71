//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_board.h"
#include "SamV71_pin_manager.h"

namespace Board
{
    class V71_EK_board : public Abstract::Abstract_board
    {
        SamV71_pin_manager
    public:
        void initialize() override;

        Pin& get_pin(Pin::Pin_id id)
        {
            return Platform::NORM_pin_manager::get_pin(id);
        }

        UART_interface& get_UART() { return the_UART; }

        Pin* get_optional_pin(Pin::Pin_id) { return nullptr; };
    };
} // Board
