//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_board.h"

namespace Board {
    class Win11 : public Abstract::Abstract_board {
    public:
        void initialize() override;

        void print_diagnostics() override;
    };
} // Board
