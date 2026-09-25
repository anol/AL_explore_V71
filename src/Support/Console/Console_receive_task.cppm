//
// Created by aeols on 2026-09-10.
//

module;
#include <cstdint>
#include <cstddef>

module Support.Console_service:Console_receive_task;
import Utility.Ringbuffer;
import Platform.FreeRTOS_task;
import Support.Abstract_command_table;
import Support.CLI_parser;
import Support.Instruction_major;
import Type.Abstract_provider;
import Type.Abstract_UART;

#include "Target_config.h"

namespace Support {
    class Console_receive_task : public FreeRTOS::FreeRTOS_task {
        enum {
            Priority            = Target_config::Console_receive_task_priority,
            Stack_size          = Target_config::Console_receive_task_stack_size,
            Queue_size          = Target_config::Console_receive_task_queue_size,
            Command_buffer_size = Target_config::Console_receive_task_buffer_size,
        };

        using Command_queue = Ringbuffer<Instruction_major, Queue_size>;
        Abstract::Abstract_UART &use_console;
        Abstract::Abstract_provider<Instruction_major> &use_router;
        Instruction::CLI_parser the_parser;
        Command_queue the_queue{};
        int escape_received{};
        volatile size_t the_buffer_pointer{};
        char the_command_buffer[Command_buffer_size]{};
        char the_previous_command[Command_buffer_size]{};
        volatile uint8_t the_rx_index{};
        bool the_echo_flag{};

    public:
        Console_receive_task(Abstract::Abstract_UART &UART, Abstract::Abstract_provider<Instruction_major> &router,
                             Instruction::Abstract_command_table &command_table)
            : FreeRTOS_task("Console receive", Priority, Stack_size), use_console(UART), use_router(router),
              the_parser(command_table.get_commands()) {
        }

        void initialize() override {
        }

        void set_echo(bool echo) { the_echo_flag = echo; };

        void task_loop() override;

    private:
        Status_code get_command(Instruction_major &instruction);

        void on_data(uint8_t data);

        bool is_command_completed(uint8_t data);

        [[nodiscard]] bool is_echo() const { return the_echo_flag; }
    };
} // Support
