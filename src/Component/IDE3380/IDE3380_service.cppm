module;
#include <cstdint>

#include "Target_config.h"

export module Component.IDE3380_service;
export import Component.IDE3380_request;
import Platform.FreeRTOS_task;
import Platform.FreeRTOS_queue;
import Type.Abstract_provider;

export namespace IDE3380 {
    /**
     * The only task that talks to the IDE3380 ASICs over SPI.
     *
     * Any task (primary, console, ...) calls execute(); the request is queued and executed here one at a
     * time by the provider, and the caller blocks until it is done. Sequences such as set ASIC + dump or
     * read-modify-write run as a single request and can never interleave with another task's transfers.
     */
    class IDE3380_service : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority   = Target_config::IDE3380_service_task_priority,
            Stack_size = Target_config::IDE3380_service_task_stack_size,
            Queue_size = Target_config::IDE3380_service_task_queue_size,
        };

        Abstract::Abstract_provider<IDE3380_request> &use_provider;
        FreeRTOS::FreeRTOS_queue<IDE3380_request *, Queue_size> the_queue{};

    public:
        explicit IDE3380_service(Abstract::Abstract_provider<IDE3380_request> &provider)
            : FreeRTOS_task("IDE3380 service", Priority, Stack_size), use_provider(provider) {
        }

        void initialize() override {
        }

        /// Queue the request and block until the service has executed it. Returns request.the_success.
        bool execute(IDE3380_request &request);

        void task_loop() override;
    };
}
