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
* @file   AT_parser.h
* @author AndersEmilOlsen, IDEAS
* @date   10.04.2026
* @brief  
*/


#pragma once

#include "Instruction/Instruction_token.h"
#include "Instruction_major.h"
#include "CLI_tokenizer.h"
#include "Status_code.h"

namespace Instruction {
    class CLI_parser {
        enum {
            Max_number_of_arguments = 16,
        };

        CLI_tokenizer::Argument the_arguments[Max_number_of_arguments]{};
        CLI_tokenizer the_tokenizer;
        const Instruction_token *const optional_root_table;
        bool is_AT_mode{};

    public:
        explicit CLI_parser(const Instruction_token *table);

        const char *parse(const char *line, Instruction_major &);

        void set_mode(bool AT_mode) {
            is_AT_mode = AT_mode;
            the_tokenizer.set_mode(AT_mode);
        };

    private:
        void parse_arguments(int argc, Instruction_major &);

        static const Instruction_token *match_token(const char *argv, const Instruction_token token[],
                                                    Instruction_major &);

        static Status_code parse_integer(const char *argv, int32_t &result, const Instruction_token *p_token,
                                  Instruction_major &);

        static Status_code parse_float(const char *argv, float32_t &result, const Instruction_token *p_token,
                                Instruction_major &);

        static Status_code parse_string(const char *argv, const char **pointer, const Instruction_token *,
                                 Instruction_major &);

        static const Instruction_token *check_match(const Instruction_token *match, Instruction_major &instruction);

        static const Instruction_token *check_final(const Instruction_token *token, Instruction_major &instruction);
    };
} // Instruction
