module;
#include <atomic>
#include <cstdint>
#include <cstdio>

module Component.IDE3380_service;
import Platform.FreeRTOS_semaphore;
import Domain.Keyword_lookup;

namespace IDE3380 {
    std::atomic<bool> IDE3380_service::the_trace_flag{false};

    bool IDE3380_service::run(Instruction_major &instruction) {
        const bool trace = is_trace();
        if (trace) {
            printf(">>>> IDE3380 ");
            instruction.print_trace(Domain_interface::get_keyword);
        }
        const bool success = use_provider.on_indication(instruction);
        if (trace) {
            printf("IDE3380 >>>> %s: ", success ? "OK" : "FAILED");
            instruction.print_trace(Domain_interface::get_keyword);
        }
        return success;
    }

    bool IDE3380_service::execute(Instruction_major &instruction) {
        if (!is_scheduler_running() || is_current_task()) {
            // Before the scheduler starts there is only one thread of execution, and from within the
            // service itself queueing would deadlock: execute directly in both cases.
            return run(instruction);
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
                transaction->the_success = run(transaction->the_instruction);
                transaction->use_done.give();
            }
        }
    }
}
