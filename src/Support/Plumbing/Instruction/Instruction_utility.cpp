/*
* Copyright (C) 2024 Integrated Detector Electronics AS
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
*
*/
//
// Created by anolsen on 08.06.2020.
//

module;
#include <cstdint>
#include <cstdio>

module Support.Instruction_utility;

using namespace Instruction;

Instruction_utility::Instruction_utility(const Instruction_token *table, func_get_keyword get_keyword) :
        optional_table(table), optional_get_keyword(get_keyword) {}

const char *Instruction_utility::to_string(const Instruction_major &instruction, char *buffer, uint32_t buffer_size) const {
    char *write_pointer = buffer;
    uint32_t number_of_tokens = instruction.get_number_of_tokens();
    int length;
    for (uint32_t index = 0; (index < number_of_tokens) && (buffer_size > 10); index++) {
        switch (instruction.get_token_type(index)) {
            case Keyword_token: {
                length = sprintf(write_pointer, "%s", optional_get_keyword(instruction.get_token_id(index)));
                if (length > 0) {
                    write_pointer += length;
                    buffer_size -= length;
                }
                break;
            }
            case Integer_token: {
                length = sprintf(write_pointer, "%ld", instruction.get_token_integer(index));
                if (length > 0) {
                    write_pointer += length;
                    buffer_size -= length;
                }
                break;
            }
            case Float_token: {
                length = sprintf(write_pointer, "%f", instruction.get_token_float(index));
                if (length > 0) {
                    write_pointer += length;
                    buffer_size -= length;
                }
                break;
            }
            case String_token: {
                length = sprintf(write_pointer, "%s", instruction.get_string());
                if (length > 0) {
                    write_pointer += length;
                    buffer_size -= length;
                }
                break;
            }
            default: {
                length = sprintf(write_pointer, "type=%u", instruction.get_token_type(index));
                if (length > 0) {
                    write_pointer += length;
                    buffer_size -= length;
                }
                break;
            }
        }
        if (index < (number_of_tokens - 1)) {
            length = sprintf(write_pointer, ",");
        } else {
            length = sprintf(write_pointer, ";\r\n");
        }
        if (length > 0) {
            write_pointer += length;
            buffer_size -= length;
        }
    }
    return buffer;
}

void Instruction_utility::print(const Instruction_major &instruction) const {
    if (optional_get_keyword) {
        for (uint32_t index = 0; index < instruction.get_number_of_tokens(); index++) {
            printf("%s", optional_get_keyword(instruction.get_token_id(index)));
            print_argument(instruction, index);
        }
        printf("\r\n");
    } else {
        dump(instruction);
    }
}

void Instruction_utility::dump(const Instruction_major &instruction) {
    printf("Instruction: ");
    for (uint32_t index = 0; index < instruction.get_number_of_tokens(); index++) {
        printf("%d", instruction.get_token_id(index));
        print_argument(instruction, index);
    }
    printf("\r\n");
}


void Instruction_utility::serialize(const Instruction_major &instruction) {
    printf("Serialize: ");
    for (uint32_t index = 0; index < instruction.get_number_of_tokens(); index++) {
        switch (instruction.get_token_type(index)) {
            case Keyword_token:
                printf("%02X ", instruction.get_token_id(index));
                break;
            case Integer_token:
            case Bitmask_token:
                printf("%02X=", instruction.get_token_id(index));
                printf("%08lX ", instruction.get_token_integer(index));
                break;
            case String_token:
            case End_token:
            default :
                printf("<> Token type error <>");
                break;
        }
    }
    printf("\r\n");
}

void Instruction_utility::print_argument(const Instruction_major &instruction, uint32_t index) {
    switch (instruction.get_token_type(index)) {
        case Keyword_token:
            break;
        case Integer_token:
            printf("=%ld", instruction.get_token_integer(index));
            break;
        case Float_token:
            printf("=%f", instruction.get_token_float(index));
            break;
        case Bitmask_token:
            printf("=0x%08lX ", instruction.get_token_integer(index));
            break;
        case String_token:
        case End_token:
        default :
            printf("<> Token type error <>");
            break;
    }
    if (++index < instruction.get_number_of_tokens()) {
        printf(", ");
    }
}
