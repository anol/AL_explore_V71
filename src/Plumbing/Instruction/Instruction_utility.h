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

#ifndef TARGET_EVAL_V71_CLI_UTILITY_H
#define TARGET_EVAL_V71_CLI_UTILITY_H

#include "Instruction_token.h"
#include "Instruction_major.h"

typedef const char *(*func_get_keyword)(uint32_t key);

namespace Instruction {

    class Instruction_utility {
        const Instruction_token *optional_table;
        func_get_keyword optional_get_keyword;

    public:
        Instruction_utility(const Instruction_token *table, func_get_keyword get_keyword);

        const char *to_string(const Instruction_major &, char *result, uint32_t buffer_size) const;

        void print(const Instruction_major &) const;

        static void dump(const Instruction_major &);

        static void serialize(const Instruction_major &);

        static void print_argument(const Instruction_major &instruction, uint32_t index);
    };
}

#endif //TARGET_EVAL_V71_CLI_UTILITY_H
