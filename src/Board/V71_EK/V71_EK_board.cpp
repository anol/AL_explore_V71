//
// Created by aeols on 12.08.2026.
//

#include "V71_EK_board.h"

#include "Abstract_clock.h"

namespace Board
{
    void V71_EK_board::initialize()
    {
        the_clock.initialize();
        the_pin_manager.initialize();
        the_UART.initialize();
        the_console.initialize();
        WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk; // Disable the watchdog
    }

    void V71_EK_board::print_diagnostics()
    {
        the_pin_manager.print_diagnostics();
    }
} // Board
