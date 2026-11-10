module;
#include <cstdint>

module Component.IDE3380_service;
import Platform.FreeRTOS_semaphore;

namespace IDE3380 {
    bool IDE3380_service::execute(Instruction_major &instruction) {
        if (!is_scheduler_running() || is_current_task()) {
            // Before the scheduler starts there is only one thread of execution, and from within the
            // service itself queueing would deadlock: execute directly in both cases.
            return use_provider.on_indication(instruction);
        }
        FreeRTOS::FreeRTOS_semaphore done{};
        Transaction transaction{.the_instruction = instruction, .use_done = done, .the_success = false};
        if (!the_queue.wait_send(&transaction)) {
            return false;
        }
        done.take();
        return transaction.the_success;
    }

    void IDE3380_service::task_loop() {
        while (true) {
            Transaction *transaction{};
            if (the_queue.wait_receive(&transaction) && transaction) {
                transaction->the_success = use_provider.on_indication(transaction->the_instruction);
                transaction->use_done.give();
            }
        }
    }
}
