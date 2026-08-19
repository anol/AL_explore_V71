//
// Created by aeols on 12.08.2026.
//

#include "V71_EK_board.h"

#include "Clock_interface.h"

namespace Board {
    void V71_EK_board::initialize() {
        Clock_interface::initialize_timers();
    }
} // Board
