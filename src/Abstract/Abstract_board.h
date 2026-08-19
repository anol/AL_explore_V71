//
// Created by aeols on 12.08.2026.
//

#pragma once

namespace Abstract {
    class Abstract_board {
    public:
        Abstract_board() = default;

        virtual ~Abstract_board() = default;

        virtual void initialize() = 0;
    };
} // Abstract
