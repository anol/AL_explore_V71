#pragma once

import Type.Abstract_target;

namespace Target {
    class V71_EK_hello_world : public Abstract::Abstract_target {
    public:
        explicit V71_EK_hello_world(Abstract::Abstract_application &application, Abstract::Abstract_board &board)
            : Abstract_target(application, board) {
        };
    };
}
