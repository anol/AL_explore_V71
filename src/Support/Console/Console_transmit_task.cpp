//
// Created by aeols on 2026-09-10.
//

module;

module Support.Console_service;

namespace Support {
    void Console_transmit_task::task_loop() {
        enum { Delay_millis = 1111 };
        while (true) {
            delay_until(Delay_millis);
        }
    }
} // Support
