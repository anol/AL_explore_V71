//
// Created by aeols on 12.08.2026.
//

module;
#include <cstdint>
#include "sam.h"
#include "cachel1_armv7.h"

module Board.V71_EK_board;
import Type.Abstract_board;
import Board.V71_EK_pin_table;
import Platform.Common_stdio;
import Platform.SamV71_clock;
import Platform.SamV71_SPI;
import Platform.SamV71_USART;
import Platform.SamV71_platform;

namespace Board {
    using namespace SamV71;

    void V71_EK_board::initialize() {
        the_clock.initialize();
        SamV71_platform::initialize();
        the_pin_table.initialize();
        the_SPI.initialize();
        the_UART.initialize();
        the_console.initialize();
    }

    void V71_EK_board::print_diagnostics() {
        the_SPI.print_diagnostics();
        the_pin_table.print_diagnostics();
    }
} // Board
