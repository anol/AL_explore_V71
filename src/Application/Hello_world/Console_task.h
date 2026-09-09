#pragma once

#include "Abstract_task.h"
#include "Cadence_control.h"
#include "Dictionary.h"
#include "CLI_parser.h"
#include "Instruction_major.h"
#include "Ringbuffer.h"


class Abstract_UART;
using namespace SpectraNode_interface;

namespace Application
{
    class Console_task : public Abstract::Abstract_task
    {
        enum { Queue_size = 16, Command_buffer_size = 100 };

        using Command_queue = Ringbuffer<Instruction_major, Queue_size>;
        Abstract_UART& use_console;
        CLI_parser the_parser;
        Command_queue the_queue{};
        int escape_received{};
        volatile size_t the_buffer_pointer{};
        char the_command_buffer[Command_buffer_size]{};
        char the_previous_command[Command_buffer_size]{};
        volatile uint8_t the_rx_index{};
        bool the_echo_flag{};

    public:
        explicit Console_task(Abstract_UART& UART) : use_console(UART), the_parser(get_commands())
        {
        }

        void initialize() override;

        bool get_command(Instruction_major& instruction);

        void ISR_on_rx(uint8_t data);

        bool is_command_completed(uint8_t data);

        void set_echo(bool echo) { the_echo_flag = echo; };

    private:
        void task_loop();

        static void task_entry(void* object);

        [[nodiscard]] bool is_echo() const { return the_echo_flag; }
    };
} // Application
