//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_application.h"
#include "Abstract_board.h"

namespace Application {
    class Hello_world : public Abstract::Abstract_application {
        Abstract::Abstract_board &use_board;

    public:
        explicit Hello_world(Abstract::Abstract_board &board) : use_board(board) {
        }

        void initialize() override;
        void delay(int number_of_loops) const;

        void run() override;
    };
} // Application
