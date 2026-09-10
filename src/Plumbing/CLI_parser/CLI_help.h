/*
* Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
* @file   CLI_help_AT.h
* @author AndersEmilOlsen, IDEAS
* @date   10.04.2026
* @brief  
*/


#pragma once

#include "Instruction/Instruction_token.h"
#include "CLI_stack.h"
#include "Status_code.h"

namespace Instruction {
    typedef const char *(*func_get_keyword)(unsigned char key);

    class CLI_help {
        uint32_t the_help_id;
        bool print_with_ids{};
        const Instruction_token *optional_table;
        func_get_keyword optional_get_keyword;
        bool is_AT_mode{};

    public:
        CLI_help(uint32_t help_id, const Instruction_token *table, func_get_keyword get_keyword);

        Status_code print(bool with_ids = false, uint32_t number_of_ids = 0);

        Status_code print(uint32_t token_id);

        void print(uint32_t command, uint32_t name);

        [[nodiscard]] bool is_help_command(uint32_t token_id) const { return token_id == the_help_id; }

        void set_mode(bool AT_mode) { is_AT_mode = AT_mode; };

    private:
        Status_code build_command(CLI_stack &stack, const Instruction_token *p_token);

        void print_command(CLI_stack &stack);

        void print_AT_command(CLI_stack &stack);

        void print_command_with_ids(CLI_stack &stack);
    };
}
