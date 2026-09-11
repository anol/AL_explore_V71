//
// Created by aeols on 2026-09-10.
//

module;
#include <cstring>
#include <cstdio>
#include <cctype>

#include "Abstract_task.h"
#include "Instruction_major.h"

module Support.Console_service;

namespace Console
{
    void Console_receive_task::initialize()
    {
        constexpr UBaseType_t priority = tskIDLE_PRIORITY + 1;
        constexpr StackType_t stack_size = 4096 * 2;
        xTaskCreate(task_entry, "Console_receive_task", stack_size, this, priority, &optional_task);
    }

    void Console_receive_task::task_entry(void* object)
    {
        static_cast<Console_receive_task*>(object)->task_loop();
    }

    void Console_receive_task::task_loop()
    {
        uint8_t data;
        Instruction_major instruction;
        while (true)
        {
            if (use_console.get(&data).success())
            {
                on_data(data);
            }
            if (get_command(instruction).success())
            {
                use_router.on_indication(instruction);
            }
        }
    }

    Status_code Console_receive_task::get_command(Instruction_major& instruction)
    {
        return Status_code(the_queue.get(&instruction));
    }

    void Console_receive_task::on_data(const uint8_t data)
    {
        constexpr char AT_prefix[] = "AT+";
        enum { Prefix_size = sizeof(AT_prefix) - 1 };
        if (is_command_completed(data))
        {
            the_buffer_pointer = 0;
            memcpy(the_previous_command, the_command_buffer, Command_buffer_size);
            Instruction_major instruction;
            const bool AT_mode{0 == strncmp(the_command_buffer, AT_prefix, Prefix_size)};
            the_parser.set_mode(AT_mode);
            if (AT_mode)
            {
                the_parser.parse(the_command_buffer + Prefix_size, instruction);
            }
            else
            {
                for (auto* string = the_command_buffer; *string; ++string)
                {
                    *string = static_cast<char>(std::toupper(static_cast<unsigned char>(*string)));
                }
                the_parser.parse(the_command_buffer, instruction);
            }
            if (instruction.is_valid())
            {
                instruction.set_AT_mode(AT_mode);
                the_queue.put(instruction);
            }
        }
    }

    bool Console_receive_task::is_command_completed(uint8_t data)
    {
        bool complete = false;
        if (the_buffer_pointer < Command_buffer_size)
        {
            if (0 < escape_received)
            {
                escape_received--;
            }
            else if (0 == the_buffer_pointer)
            {
                // Initial character.
                if (' ' <= data && '~' >= data)
                {
                    // Space, numbers, letters, or symbols.
                    if (is_echo())
                    {
                        printf("\r%c", data);
                    }
                    the_command_buffer[the_buffer_pointer] = static_cast<char>(data);
                    the_buffer_pointer = the_buffer_pointer + 1;
                }
                else if (0x1Bu == data)
                {
                    // Escape
                    escape_received = 2;
                    memcpy(the_command_buffer, the_previous_command, Command_buffer_size);
                    the_buffer_pointer = strlen(the_command_buffer);
                    if (is_echo())
                    {
                        printf("\r");
                        printf(the_command_buffer);
                    }
                }
                else if (0x7Fu != data)
                {
                    // Delete.
                    if (is_echo())
                    {
                        printf("\r \r\n");
                    }
                }
            }
            else
            {
                // Following characters
                if ((' ' <= data) && ('~' >= data) && (';' != data))
                {
                    // Space, numbers, letters, or symbols, except command terminator.
                    if (is_echo())
                    {
                        printf("%c", data);
                    }
                    the_command_buffer[the_buffer_pointer] = static_cast<char>(data);
                    the_buffer_pointer = the_buffer_pointer + 1;
                }
                else if (0x8u == data || 0x7Fu == data)
                {
                    // Backspace or delete.
                    if (is_echo())
                    {
                        printf("\b \b");
                    }
                    the_buffer_pointer = the_buffer_pointer - 1;
                }
                else if (0xAu == data || 0xDu == data || ';' == data)
                {
                    // LF, CR, or command terminator.
                    if (is_echo())
                    {
                        printf("\r\n");
                    }
                    the_command_buffer[the_buffer_pointer] = 0x0u;
                    the_buffer_pointer = 0;
                    complete = true;
                }
            }
        }
        else
        {
            // error_code(Too_long_command, "To long command");
            the_buffer_pointer = 0;
            complete = true;
        }
        return complete;
    }
} // Console
