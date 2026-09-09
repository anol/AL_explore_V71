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
* @file   Command_parser.h
* @author AndersEmilOlsen, IDEAS
* @date   14.04.2026
* @brief  
*/


#pragma once

#include "Cadence_control.h"

#include "Dictionary.h"
#include "CLI_parser.h"
#include "Instruction_major.h"
#include "Ringbuffer.h"

using namespace SpectraNode_interface;

namespace Application {
    class Command_parser {
        enum { Queue_size = 16, Command_buffer_size = 100 };

        using Command_queue = Ringbuffer<Instruction_major, Queue_size>;
        CLI_parser the_parser{get_commands()};
        Command_queue the_queue{};
        int escape_received{};
        volatile size_t the_buffer_pointer{};
        char the_command_buffer[Command_buffer_size]{};
        char the_previous_command[Command_buffer_size]{};
        volatile uint8_t the_rx_index{};
        bool the_echo_flag{};

    public:
        void initialize();

        bool get_command(Instruction_major &instruction);

        void ISR_on_rx(uint8_t data);

        bool is_command_completed(uint8_t data);

        void set_echo(bool echo) { the_echo_flag = echo; };

    private:
        [[nodiscard]] bool is_echo() const { return the_echo_flag; }
    };
} // Application
