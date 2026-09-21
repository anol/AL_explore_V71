//
// Created by aeols on 12.08.2026.
//

#pragma once
import Type.Abstract_board;
#include "Win11_pin_manager.h"
#include "Win11_UART.h"

namespace Board
{
    class Win11_board : public Abstract::Abstract_board
    {
        Win11::Win11_pin_manager the_pin_manager{};
        Win11::Win11_UART the_UART{};

    public:
        void initialize() override;

        void print_diagnostics() override;

        void NOP() override
        {
        }

        Abstract_UART& get_UART() override { return the_UART; }

        Abstract::Abstract_IO_pin& get_pin(const Dictionary::Pin_id id) override
        {
            return the_pin_manager.get_pin(id);
        }
    };
} // Board
