//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_board.h"
#include "Common_stdio.h"
#include "SamV71_clock.h"
#include "SamV71_pin_manager.h"
#include "SamV71_USART1.h"

namespace Board
{
    using namespace SamV71;

    class V71_EK_board : public Abstract::Abstract_board
    {
        SamV71_clock the_clock{};
        SamV71_pin_manager the_pin_manager{};
        SamV71_USART1 the_UART{};
        Platform::Common_stdio the_console{&the_UART};

    public:
        V71_EK_board() = default;

        void initialize() override;

        void print_diagnostics() override;

        Abstract::Abstract_UART& get_UART() override { return the_UART; }

        Abstract::Abstract_IO_pin& get_pin(const Dictionary::Pin_id id) override
        {
            return the_pin_manager.get_pin(id);
        }

    private:
        static void enable_cache();
    };
} // Board
