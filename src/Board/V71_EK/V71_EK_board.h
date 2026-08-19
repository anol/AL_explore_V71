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
        SamV71_UART the_UART;
    public:
        void initialize() override;

        SamV71_pin_manager::Pin& get_pin(SamV71_pin_manager::Pin::Pin_id id)
        {
            return Platform::SamV71_pin_manager::get_pin(id);
        }

        Abstract_UART& get_UART() { return the_UART; }

        SamV71_pin_manager::Pin* get_optional_pin(SamV71_pin_manager::Pin::Pin_id) { return nullptr; };
    };
} // Board
