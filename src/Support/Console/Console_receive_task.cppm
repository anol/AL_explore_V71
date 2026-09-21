//
// Created by aeols on 2026-09-10.
//

module;
#include "FreeRTOS_task.h"
#include "CLI_parser.h"
#include "Instruction_major.h"
#include "Ringbuffer.h"
#include "Status_code.h"
#include "Abstract_UART.h"
#include "Abstract_provider.h"

module Support.Console_service:Console_receive_task;

namespace Console
{
    class Console_receive_task : public FreeRTOS::FreeRTOS_task
    {
        enum { Queue_size = 16, Command_buffer_size = 100, Priority = Default_priority, Stack_size = 4096 * 2 };

        using Command_queue = Ringbuffer<Instruction_major, Queue_size>;
        Abstract::Abstract_UART& use_console;
        Abstract::Abstract_provider& use_router;
        CLI_parser the_parser;
        Command_queue the_queue{};
        int escape_received{};
        volatile size_t the_buffer_pointer{};
        char the_command_buffer[Command_buffer_size]{};
        char the_previous_command[Command_buffer_size]{};
        volatile uint8_t the_rx_index{};
        bool the_echo_flag{};

    public:
        Console_receive_task(Abstract::Abstract_UART& UART, Abstract::Abstract_provider& router)
            : FreeRTOS_task("Console receive", Priority, Stack_size), use_console(UART), use_router(router),
              the_parser(get_commands())
        {
        }

        void initialize() override
        {
        }

        void set_echo(bool echo) { the_echo_flag = echo; };

        void task_loop() override;

    private:
        Status_code get_command(Instruction_major& instruction);

        void on_data(uint8_t data);

        bool is_command_completed(uint8_t data);

        [[nodiscard]] bool is_echo() const { return the_echo_flag; }
    };
} // Console
