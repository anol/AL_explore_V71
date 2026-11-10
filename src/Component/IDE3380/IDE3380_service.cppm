module;
#include <atomic>
#include <cstdint>

#include "Target_config.h"

export module Component.IDE3380_service;
export import Support.Instruction_major;
import Platform.FreeRTOS_task;
import Platform.FreeRTOS_queue;
import Type.Abstract_provider;
import Type.Abstract_semaphore;

export namespace IDE3380 {
    /**
     * The only task that talks to the IDE3380 ASICs over SPI.
     *
     * Any task (primary, console, ...) calls execute() with an Instruction_major built from Domain keywords
     * (see IDE3380_interface for the grammar). The instruction is queued and executed here one at a time by
     * the provider, and the caller blocks until it is done. Results are added to the instruction as tokens.
     */
    class IDE3380_service : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority = Target_config::IDE3380_service_task_priority,
            Stack_size = Target_config::IDE3380_service_task_stack_size,
            Queue_size = Target_config::IDE3380_service_task_queue_size,
        };

        /// Lives on the caller's stack while the caller blocks.
        struct Transaction {
            Instruction_major &the_instruction;
            Abstract::Abstract_semaphore &use_done;
            bool the_success;
        };

        static std::atomic<bool> the_trace_flag;

        Abstract::Abstract_provider<Instruction_major> &use_provider;
        FreeRTOS::FreeRTOS_queue<Transaction *, Queue_size> the_queue{};

    public:
        explicit IDE3380_service(Abstract::Abstract_provider<Instruction_major> &provider)
            : FreeRTOS_task("IDE3380 service", Priority, Stack_size), use_provider(provider) {}

        void initialize() override {}

        /// Queue the instruction and block until the service has executed it. Returns the provider's verdict.
        bool execute(Instruction_major &instruction);

        void task_loop() override;

        /// When on, every executed instruction is printed before (">") and after ("<", with results) execution.
        static void set_trace(const bool trace) { the_trace_flag = trace; }

        static bool is_trace() { return the_trace_flag; }

    private:
        bool run(Instruction_major &instruction);
    };
}
