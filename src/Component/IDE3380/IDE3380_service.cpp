module;
#include <cstdint>

module Component.IDE3380_service;
import Platform.FreeRTOS_semaphore;

namespace IDE3380 {
    bool IDE3380_service::execute(IDE3380_request &request) {
        if (!is_scheduler_running() || is_current_task()) {
            // Before the scheduler starts there is only one thread of execution, and from within the
            // service itself queueing would deadlock: execute directly in both cases.
            request.the_success = use_provider.on_indication(request);
            return request.the_success;
        }
        FreeRTOS::FreeRTOS_semaphore done{};
        request.optional_done = &done;
        request.the_success = false;
        if (!the_queue.wait_send(&request)) {
            return false;
        }
        done.take();
        return request.the_success;
    }

    void IDE3380_service::task_loop() {
        while (true) {
            IDE3380_request *request{};
            if (the_queue.wait_receive(&request) && request) {
                request->the_success = use_provider.on_indication(*request);
                if (auto *done = request->optional_done) {
                    done->give();
                }
            }
        }
    }
}
