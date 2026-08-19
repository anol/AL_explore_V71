//
// Created by aeols on 12.08.2026.
//

#pragma once
#include "Abstract_application.h"
#include "Abstract_board.h"

namespace Abstract {
    class Abstract_target {
        Abstract_application &use_application;
        Abstract_board &use_board;

    public:
        Abstract_target(Abstract_application &application, Abstract_board &board)
            : use_application(application), use_board(board) {
        };

        virtual ~Abstract_target() = default;

        virtual void initialize() const {
            use_board.initialize();
            use_application.initialize();
        }

        virtual void run() const { use_application.run(); }
        [[nodiscard]] Abstract_application &get_application() const { return use_application; }
        [[nodiscard]] Abstract_board &get_board() const { return use_board; }
    };
} // Abstract
