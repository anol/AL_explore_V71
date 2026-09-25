//
// Created by aeols on 2026-09-10.
//

module;

module Support.Console_service:Console_transmit_task;
import Platform.FreeRTOS_task;

#include "Target_config.h"

namespace Support {
    class Console_transmit_task : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority            = Target_config::Console_transmit_task_priority,
            Stack_size          = Target_config::Console_transmit_task_stack_size,
         };

    public:
        Console_transmit_task() : FreeRTOS_task("Console transmit", Priority, Stack_size) {
        }

        void initialize() override {
        };

        void task_loop() override;
    };
} // Support
