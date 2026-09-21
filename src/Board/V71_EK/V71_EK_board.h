//
// Created by aeols on 12.08.2026.
//

#pragma once
import Type.Abstract_board;
#include "Common_stdio.h"
#include "SamV71_clock.h"
#include "SamV71_SPI.h"
#include "V71_EK_pin_manager.h"
#include "SamV71_USART1.h"

namespace Board {
    using namespace SamV71;
    using namespace Abstract;

    class V71_EK_board : public Abstract_board {
        SamV71_clock           the_clock{};
        V71_EK_pin_manager     the_pin_manager{};
        SamV71_SPI             the_SPI{};
        SamV71_USART1          the_UART{};
        Platform::Common_stdio the_console{&the_UART};

    public:
        V71_EK_board() = default;

        void initialize() override;

        void print_diagnostics() override;

        Abstract_UART &  get_UART() override { return the_UART; }
        Abstract_SPI &   get_SPI() override { return the_SPI; }
        Abstract_IO_pin &get_pin(const Dictionary::Pin_id id) override { return the_pin_manager.get_pin(id); }

    private:
        static void enable_cache();
    };
} // Board
