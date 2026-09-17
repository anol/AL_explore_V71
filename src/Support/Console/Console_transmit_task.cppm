//
// Created by aeols on 2026-09-10.
//

module;
#include "FreeRTOS_task.h"

module Support.Console_service:Console_transmit_task;

namespace Console {
    class Console_transmit_task : public FreeRTOS::FreeRTOS_task {
    public:
        Console_transmit_task() : FreeRTOS_task("Console transmit") {
        }

        void initialize() override {
        };

        void task_loop() override;
    };
} // Console
