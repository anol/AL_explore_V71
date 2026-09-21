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

#ifndef TARGET_WINDOWS_CLI_stack_H
#define TARGET_WINDOWS_CLI_stack_H

#include <cstdint>

#include "Instruction/Instruction_token.h"
import Type.Status_code;

namespace Instruction {

    class CLI_stack {
        enum {
            Max_number_of_token = 16
        };
        uint32_t number_of_tokens{};
        Instruction_token my_tokens[Max_number_of_token]{};

    public:
        Status_code push(const Instruction_token &token);

        Status_code pop(Instruction_token &token);

        Status_code get_token(uint32_t index, Instruction_token &token);

        void clear() { number_of_tokens = 0; }
    };

}

#endif //TARGET_WINDOWS_CLI_stack_H
