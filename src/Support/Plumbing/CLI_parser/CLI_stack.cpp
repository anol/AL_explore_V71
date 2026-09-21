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
// Created by anolsen on 23.04.2020.
//

module;
#include <cstdint>

module Support.CLI_stack;

using namespace Instruction;

Status_code CLI_stack::push(const Instruction_token &token) {
    if (number_of_tokens < (Max_number_of_token - 1)) {
        my_tokens[number_of_tokens++] = token;
        return Status_code::Success();
    } else {
        return Status_code::Failure();
    }
}

Status_code CLI_stack::pop(Instruction_token &token) {
    if ((number_of_tokens > 0) && (number_of_tokens < (Max_number_of_token))) {
        token = my_tokens[number_of_tokens];
        my_tokens[number_of_tokens].clear();
        number_of_tokens--;
        return Status_code::Success();
    } else {
        return Status_code::Failure();
    }
}

Status_code CLI_stack::get_token(uint32_t index, Instruction_token &token) {
    if (index < number_of_tokens) {
        token = my_tokens[index];
        return Status_code::Success();
    } else {
        return Status_code::Failure();
    }
}
