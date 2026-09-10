//
// Created by aeols on 12.08.2026.
//

#include "V71_EK_board.h"

namespace Board
{
    void V71_EK_board::initialize()
    {
        the_clock.initialize();
        enable_cache();
        the_pin_manager.initialize();
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
        // the_pin_manager.print_diagnostics();
    }
} // Board
