//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_board.h"
#include "SamV71_pin_manager.h"
#include "SamV71_UART.h"

namespace Board
{
    class V71_EK_board : public Abstract::Abstract_board
    {
        SamV71::SamV71_UART the_UART;

    public:
        void initialize() override;

        Abstract_UART& get_UART() { return the_UART; }

        Abstract::Abstract_IO_pin& get_pin(const Application_configuration::Pin_id id)
        {
            return SamV71_pin_manager::get_pin(id);
        }

        Abstract::Abstract_IO_pin* get_optional_pin(Application_configuration::Pin_id)
        {
            return nullptr;
        };
    };
} // Board
