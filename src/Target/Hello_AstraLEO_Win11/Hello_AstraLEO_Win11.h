//
// Created by aeols on 12.08.2026.
//

#pragma once
import Type.Abstract_target;

namespace Target {
    class Hello_AstraLEO_Win11 : public Abstract::Abstract_target{
    public:
        Hello_AstraLEO_Win11(Abstract::Abstract_application &application, Abstract::Abstract_board &board)
            : Abstract_target(application, board) {
        }
    };
} // Target
