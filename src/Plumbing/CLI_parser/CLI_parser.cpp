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
* @file   CLI_parser.cpp
* @author AndersEmilOlsen, IDEAS
* @date   10.04.2026
* @brief  
*/

#include <cstdlib>
#include <cstring>

#include "CLI_parser.h"

#include "Simple_string.h"

namespace Instruction {
    static Instruction_token question_mark(Question_mark, "?", 0, static_cast<uint32_t>(Special_command), nullptr);

    CLI_parser::CLI_parser(const Instruction_token *table) : the_tokenizer(the_arguments, Max_number_of_arguments),
                                                             optional_root_table(table) {
    }

    const char *CLI_parser::parse(const char *line, Instruction_major &instruction) {
        instruction.clean();
        if (line != nullptr) {
            int argc;
            line = the_tokenizer.tokenize(line, &argc);
            if (argc > 0) {
                parse_arguments(argc, instruction);
            }
        }
        return line;
    }

    void CLI_parser::parse_arguments(int argc, Instruction_major &instruction) {
        int argument = 0;
        const Instruction_token *token = optional_root_table;
        while ((argc-- > 0) && (token != nullptr)) {
            const char *argv = the_arguments[argument++].text;
            auto *match = match_token(argv, token, instruction);
            token = check_match(match, instruction);
        }
        while (token) {
            token = check_final(token, instruction);
        }
    }

    const Instruction_token *
    CLI_parser::check_match(const Instruction_token *match, Instruction_major &instruction) {
        const Instruction_token *token{};
        if (match) {
            switch (match->token_type) {
                case End_token:
                case Help_text:
                case Question_mark: {
                    instruction.close(match->the_value);
                    break;
                }
                case Command_token:
                case Keyword_token:
                case Integer_token:
                case Bitmask_token:
                case String_token:
                default: {
                    token = match->optional_next;
                    if (!token) {
                        instruction.missing_argument();
                    }
                    break;
                }
            }
        }
        return token;
    }

    const Instruction_token *
    CLI_parser::check_final(const Instruction_token *token, Instruction_major &instruction) {
        if (token) {
            if (token->token_type == Help_text) {
                instruction.close(token->the_value);
                token = nullptr;
            } else if (token->token_type == End_token) {
                instruction.missing_argument();
                token = nullptr;
            } else {
                token++;
            }
        }
        return token;
    }

    const Instruction_token *CLI_parser::match_token(const char *argv, const Instruction_token *token,
                                                     Instruction_major &instruction) {
        const Instruction_token *match{};
        if (argv != nullptr) {
            const char *string{};
            int32_t i_value{};
            float32_t f_value{};
            uint32_t cnt_match = 0;
            Token_type match_type{};
            size_t wordlen = strlen(argv);
            while (token != nullptr) {
                switch (token->token_type) {
                    case Keyword_token:
                        if (token->optional_keyword && (strncmp(argv, token->optional_keyword, wordlen) == 0)) {
                            match_type = Keyword_token;
                            match = token;
                            cnt_match++;
                        }
                        break;
                    case Integer_token:
                        if (parse_integer(argv, i_value, token, instruction)) {
                            match_type = Integer_token;
                            match = token;
                            cnt_match++;
                        }
                        break;
                    case Float_token:
                        if (parse_float(argv, f_value, token, instruction)) {
                            match_type = Float_token;
                            match = token;
                            cnt_match++;
                        }
                        break;
                    case Bitmask_token:
                        if (Simple_string::stringmask_to_bitmask(argv, i_value)) {
                            match_type = Integer_token;
                            match = token;
                            cnt_match++;
                        }
                        break;
                    case String_token:
                        if (parse_string(argv, &string, token, instruction)) {
                            match_type = String_token;
                            match = token;
                            cnt_match++;
                        }
                        break;
                    case End_token:
                    default:
                        token = nullptr;
                        break;
                }
                if (token != nullptr) {
                    token++;
                }
            }
            if (cnt_match == 1) {
                switch (match_type) {
                    case Integer_token: {
                        Instruction_token integer_token(match, i_value);
                        instruction.add_token(integer_token);
                        break;
                    }
                    case Float_token: {
                        Instruction_token float_token(match, f_value);
                        instruction.add_token(float_token);
                        break;
                    }
                    case String_token: {
                        Instruction_token string_token(match, string);
                        instruction.add_token(string_token);
                        break;
                    }
                    default: {
                        Instruction_token default_token(match, (int32_t) 0);
                        instruction.add_token(default_token);
                        break;
                    }
                }
            } else if (cnt_match == 0) {
                if (wordlen == 1 && *argv == '?') {
                    instruction.add_token(question_mark);
                    match = &question_mark;
                } else {
                    instruction.unknown_argument(argv);
                    match = nullptr;
                }
            } else {
                instruction.ambiguous_argument(argv);
                match = nullptr;
            }
        }
        return match;
    }

    bool CLI_parser::parse_integer(const char *argv, int32_t &result, const Instruction_token *p_token,
                                   Instruction_major &instruction) {
        if (strlen(argv) > 2 && argv[0] == '0' && argv[1] == 'x') {
            result = static_cast<int>(strtoul(argv, nullptr, 0));
            auto tmp = static_cast<uint32_t>(result);
            auto success = ((tmp >= static_cast<uint32_t>(p_token->the_min_value)) && (
                                tmp <= static_cast<uint32_t>(p_token->the_max_value)));
            if (!success) {
                instruction.value_out_of_range();
            }
            return success;
        }
        if (strlen(argv) > 0 && ((argv[0] >= '0' && argv[0] <= '9') || (argv[0] == '-' || argv[0] <= '+'))) {
            result = strtol(argv, nullptr, 0);
            auto success = result >= p_token->the_min_value && result <= p_token->the_max_value;
            if (!success) {
                instruction.value_out_of_range();
            }
            return success;
        }
        return false;
    }

    bool CLI_parser::parse_float(const char *argv, float32_t &result, const Instruction_token *p_token,
                                 Instruction_major &instruction) {
        if (strlen(argv) > 0 && ((argv[0] >= '0' && argv[0] <= '9') || (argv[0] == '-' || argv[0] <= '+'))) {
            result = strtof(argv, nullptr);
            auto success = ((result >= (float32_t) p_token->the_min_value) &&
                            (result <= (float32_t) p_token->the_max_value));
            if (!success) {
                instruction.value_out_of_range();
            }
            return success;
        }
        return false;
    }

    bool CLI_parser::parse_string(const char *argv, const char **pointer, const Instruction_token *p_token,
                                  Instruction_major &instruction) {
        bool success = false;
        *pointer = nullptr;
        auto length = strlen(argv);
        if ((length <= Instruction_token::Max_string_length) && (length <= (uint32_t) p_token->the_max_value)) {
            *pointer = argv;
            success = true;
        } else {
            instruction.oversized_string(argv, length, p_token->the_max_value);
        }
        return success;
    }
} // Instruction
