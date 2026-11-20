#pragma once

import Type.Abstract_target;

namespace Target {
    class Hello_AstraLEO_EM : public Abstract::Abstract_target {
    public:
        explicit Hello_AstraLEO_EM(Abstract::Abstract_application &application, Abstract::Abstract_board &board)
            : Abstract_target(application, board) {
        };
    };
}
