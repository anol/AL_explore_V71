module;

export module Support.Heartbeat_task;
import Platform.FreeRTOS_task;
import Type.Abstract_IO_pin;

#include "Target_config.h"

namespace Support {
    using namespace Abstract;

    export class Heartbeat_task : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority   = Target_config::Heartbeat_task_priority,
            Stack_size = Target_config::Heartbeat_task_stack_size
        };

        Abstract_IO_pin &use_pin_A;
        Abstract_IO_pin &use_pin_B;
        bool the_toggle_flag{};

    public:
        Heartbeat_task(Abstract_IO_pin &A, Abstract_IO_pin &B)
            : FreeRTOS_task("Console receive", Priority, Stack_size),
              use_pin_A(A), use_pin_B(B) {
        };

        void initialize() override;

        void task_loop() override;

    private:
        void toggle_LED();
    };
} // Support
