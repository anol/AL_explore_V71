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
* @file   CLI_help.cpp
* @author AndersEmilOlsen, IDEAS
* @date   10.04.2026
* @brief  
*/


#include <cstring>


#include "CLI_help.h"

#include "Dictionary.h"
#include "SpectraNode_error_code.h"
#include <stdio.h>

using namespace Instruction;
using namespace Dictionary;

CLI_help::CLI_help(uint32_t help_id, const Instruction_token *table, func_get_keyword get_keyword)
    : the_help_id(help_id), optional_table(table), optional_get_keyword(get_keyword) {
}

// print
// - Only the significant letters are necessary.
// - Use ? as 2nd argument for help on an individual command.
// - Use 'help ?' to display the numeric token codes.
Error_code CLI_help::print(bool with_ids, uint32_t number_of_ids) {
    Error_code result{Success_code};
    print_with_ids = with_ids;
    CLI_stack stack{};
    const Instruction_token *p_token = optional_table;
    while (p_token != nullptr && p_token->token_type != End_token) {
        stack.clear();
        if (stack.push(*p_token)) {
            result = build_command(stack, p_token->optional_next);
            p_token++;

            if (result.failed()) {
                break;
            }
        } else {
            p_token = nullptr;
        }
    }
    if (with_ids) {
        printf("\r\nThe keywords in numeric order\r\n");
        for (uint32_t i = 0; i < number_of_ids; ++i) {
            printf(" 0x%02lX %s\r\n", i, optional_get_keyword(i));
        }
    }
    printf("\r\n\r\n");
    return result;
}

Error_code CLI_help::print(uint32_t token_id) {
    Error_code result{Success_code};
    CLI_stack stack{};
    const Instruction_token *p_token = optional_table;
    printf("\r\n");
    while (p_token != nullptr && p_token->token_type != End_token) {
        if (p_token->the_id == token_id) {
            stack.clear();
            if (stack.push(*p_token)) {
                result = build_command(stack, p_token->optional_next);
                p_token++;
                if (result.failed()) {
                    break;
                }
            } else {
                p_token = nullptr;
            }
        } else {
            p_token++;
        }
    }
    printf("\r\n");
    return result;
}

void CLI_help::print(uint32_t command, uint32_t name) {
    while (command) {
        printf("%s ", optional_get_keyword(command & 0xFFu));
        command >>= 8u;
    }
    while (name) {
        printf("%c", static_cast<char>(name & 0xFFu));
        name >>= 8u;
    }
    printf("\r\n");
}

Error_code CLI_help::build_command(CLI_stack &stack, const Instruction_token *p_token) {
    Error_code result{Success_code};
    if (p_token == nullptr || p_token->token_type == End_token) {
        if (print_with_ids) {
            print_command_with_ids(stack);
        } else if (is_AT_mode) {
            print_AT_command(stack);
        } else {
            print_command(stack);
        }
    } else {
        Instruction_token token{};
        while (p_token != nullptr && p_token->token_type != End_token) {
            if (stack.push(*p_token)) {
                result = build_command(stack, p_token->optional_next);
                stack.pop(token);
                p_token++;

                if (result.failed()) {
                    break;
                }
            } else {
                result = Token_limit_exceeded;
                p_token = nullptr;
            }
        }
    }
    return result;
}

void CLI_help::print_command(CLI_stack &stack) {
    uint32_t index = 0;
    Instruction_token token;
    while (stack.get_token(index++, token)) {
        if (token.optional_keyword && strlen(token.optional_keyword)) {
            if (index == 1) {
                printf(" %-10s", token.optional_keyword);
            } else if (token.token_type == Help_text) {
                for (uint32_t cnt = 6; cnt > index; cnt--) printf("\t");
                printf("\t// %s", token.optional_keyword);
            } else if (token.optional_keyword[0] != '_') {
                printf(" %s", token.optional_keyword);
            }
        } else if (token.the_id > 0) {
            printf(" <%s>", optional_get_keyword(token.the_id));
        }
    }
    printf("\r\n");
}

void CLI_help::print_AT_command(CLI_stack &stack) {
    uint32_t index = 0;
    bool first_param{};
    Instruction_token token;
    while (stack.get_token(index++, token)) {
        if (token.optional_keyword && strlen(token.optional_keyword)) {
            if (index == 1) {
                first_param = true;
                printf("AT+%s", token.optional_keyword);
            } else if (token.token_type == Help_text) {
                // for (uint32_t cnt = 6; cnt > index; cnt--) printf("\t");
                printf("; \t// %s", token.optional_keyword);
            } else if (token.optional_keyword[0] != '_') {
                printf("_%s", token.optional_keyword);
            }
        } else if (token.the_id > 0) {
            switch (token.token_type) {
                case Bitmask_token: {
                    break;
                }
                default: {
                    if (first_param) {
                        first_param = false;
                        printf("=<%s>", optional_get_keyword(token.the_id));
                    } else {
                        printf(",<%s>", optional_get_keyword(token.the_id));
                    }
                    break;
                }
            }
        }
    }
    printf("\r\n");
}

void CLI_help::print_command_with_ids(CLI_stack &stack) {
    uint32_t index = 0;
    Instruction_token token;
    while (stack.get_token(index++, token)) {
        if (token.optional_keyword && strlen(token.optional_keyword)) {
            if (index == 1) {
                printf(" [0x%02X]%-10s", token.the_id, token.optional_keyword);
            } else if (token.token_type == Help_text) {
                for (uint32_t cnt = 6; cnt > index; cnt--) printf("\t");
                printf("\t// %s", token.optional_keyword);
            } else if (token.optional_keyword[0] != '_') {
                printf(" [0x%02X]%s", token.the_id, token.optional_keyword);
            }
        } else if (token.the_id > 0) {
            printf(" [0x%02X]<%s>", token.the_id, optional_get_keyword(token.the_id));
        }
    }
    printf("\r\n");
}
