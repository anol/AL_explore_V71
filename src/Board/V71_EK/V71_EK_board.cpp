//
// Created by aeols on 12.08.2026.
//

#include "V71_EK_board.h"

#include "Abstract_clock.h"

namespace Board {
    void V71_EK_board::initialize() {
        the_clock.initialize();
        the_UART.initialize();
    }
} // Board
