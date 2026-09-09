/*
* Copyright (C) 2020-2026 Integrated Detector Electronics AS
* All Rights Reserved.
*
* NOTICE: All information contained herein is, and remains
* the property of Integrated Detector Electronics AS and its suppliers,
* if any. The intellectual and technical concepts contained
* herein are proprietary to Integrated Detector Electronics AS
* and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Integrated Detector Electronics AS.
*/

/**
* @file   Command_parser.cpp
* @author AndersEmilOlsen, IDEAS
* @date   14.04.2026
* @brief  
*/


#include "Command_parser.h"

#include <cstdlib>
#include <cstring>

#include <Diagnostic.h>

#include "Dictionary.h"
#include "CLI_help.h"
#include "CLI_parser.h"
#include "Instruction_major.h"

#include "stdio.h"
// #include "Device/MCU/STM32U575RG/U575xx_USB_serial.h"

#include <cctype>

using namespace SpectraNode_interface;

namespace Application {
    void Command_parser::initialize() {
        // STM32U575RG::U575xx_USB_serial::initialize(this, [](void *user, uint8_t data) {
        //     if (user) {
        //         static_cast<Command_parser *>(user)->ISR_on_rx(data);
        //     }
        // });
    }

    bool Command_parser::get_command(Instruction_major &instruction) {
        // STM32U575RG::U575xx_USB_serial::get_state();
        return the_queue.get(&instruction);
    }

    void Command_parser::ISR_on_rx(uint8_t data) {
        constexpr char AT_prefix[] = "AT+";
        enum { Prefix_size = sizeof(AT_prefix) - 1 };
        bool complete{is_command_completed(data)};
        if (complete) {
            the_buffer_pointer = 0;
            memcpy(the_previous_command, the_command_buffer, Command_buffer_size);
            Instruction_major instruction;
            bool AT_mode{0 == strncmp(the_command_buffer, AT_prefix, Prefix_size)};
            the_parser.set_mode(AT_mode);
            if (AT_mode) {
                the_parser.parse(the_command_buffer + Prefix_size, instruction);
            } else {
                auto *string = the_command_buffer;
                for (; *string; ++string) {
                    *string = static_cast<char>(std::toupper(static_cast<unsigned char>(*string)));
                }
                the_parser.parse(the_command_buffer, instruction);
            }
            if (instruction.is_valid()) {
                instruction.set_AT_mode(AT_mode);
                the_queue.put(instruction);
            }
        }
    }

    bool Command_parser::is_command_completed(uint8_t data) {
        bool complete = false;
        if (the_buffer_pointer < Command_buffer_size) {
            if (0 < escape_received) {
                escape_received--;
            } else if (0 == the_buffer_pointer) {
                // Initial character.
                if ((' ' <= data) && ('~' >= data)) {
                    // Space, numbers, letters, or symbols.
                    if (is_echo()) printf("\r%c", data);
                    the_command_buffer[the_buffer_pointer] = static_cast<char>(data);
                    the_buffer_pointer = the_buffer_pointer + 1;
                } else if (0x1Bu == data) {
                    // Escape
                    escape_received = 2;
                    memcpy(the_command_buffer, the_previous_command, Command_buffer_size);
                    the_buffer_pointer = strlen(the_command_buffer);
                    if (is_echo()) printf("\r");
                    if (is_echo()) printf(the_command_buffer);
                } else if (0x7Fu != data) {
                    // Delete.
                    if (is_echo()) printf("\r \r\n");
                }
            } else {
                // Following characters
                if ((' ' <= data) && ('~' >= data) && (';' != data)) {
                    // Space, numbers, letters, or symbols, except command terminator.
                    if (is_echo()) printf("%c", data);
                    the_command_buffer[the_buffer_pointer] = static_cast<char>(data);
                    the_buffer_pointer = the_buffer_pointer + 1;
                } else if (0x8u == data || 0x7Fu == data) {
                    // Backspace or delete.
                    if (is_echo()) printf("\b \b");
                    the_buffer_pointer = the_buffer_pointer - 1;
                } else if ((0xAu == data) || (0xDu == data) || (';' == data)) {
                    // LF, CR, or command terminator.
                    if (is_echo()) printf("\r\n");
                    the_command_buffer[the_buffer_pointer] = 0x0u;
                    the_buffer_pointer = 0;
                    complete = true;
                }
            }
        } else {
            // error_code(Too_long_command, "To long command");
            the_buffer_pointer = 0;
            complete = true;
        }
        return complete;
    }
} // Application
