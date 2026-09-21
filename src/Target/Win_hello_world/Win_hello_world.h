//
// Created by aeols on 12.08.2026.
//

#pragma once
import Type.Abstract_target;

namespace Target {
    class Windows_hello_world : public Abstract::Abstract_target{
    public:
        Windows_hello_world(Abstract::Abstract_application &application, Abstract::Abstract_board &board)
            : Abstract_target(application, board) {
        }
    };
} // Target
