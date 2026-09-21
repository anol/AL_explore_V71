//
// Created by aeols on 12.08.2026.
//

module;
#include <cstdint>
#include "Common_stdio.h"
#include "SamV71_clock.h"
#include "SamV71_SPI.h"
#include "SamV71_USART1.h"
#include "sam.h"
#include "cachel1_armv7.h"

module Board.V71_EK_board;
import Type.Abstract_board;
import Board.V71_EK_pin_table;

namespace Board
{
    void V71_EK_board::initialize()
    {
        the_clock.initialize();
        enable_cache();
        the_pin_table.initialize();
        the_SPI.initialize();
        the_UART.initialize();
        the_console.initialize();
    }

    void V71_EK_board::enable_cache()
    {
        SCB_EnableICache();
        SCB_EnableDCache();
    }

    void V71_EK_board::print_diagnostics()
    {
        // the_pin_table.print_diagnostics();
    }
} // Board
