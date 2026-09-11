/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 * \date   IDEAS/26.10.2020/aeols
 * \brief
 */

#ifndef TARGET_TOOLS_INSTRUCTION_LOOKUP_H
#define TARGET_TOOLS_INSTRUCTION_LOOKUP_H

#include <Diagnostic.h>
#include <Instruction/Instruction_major.h>

template<class T>
class Instruction_lookup {
public:
    typedef void (T::*Instruction_handler)(Instruction_major &instruction);

    struct Instruction_entry {
        Instruction_handler optional_handler;
        uint8_t the_command_id;
    };

    static const Instruction_entry void_instruction;
    static const Instruction_entry instruction_table[];

    static Instruction_handler find_instruction_entry(const uint8_t command_id) {
        for (auto &entry: instruction_table) {
            if (entry.the_command_id == command_id) {
                return entry.optional_handler;
            }
        }
        return nullptr;
    }

    static bool lookup(T *user, Instruction_major &instruction) {
        auto handler = find_instruction_entry(instruction.get_command_id());
        auto success = handler != nullptr;
        if (success) {
            (user->*handler)(instruction);
        }
        return success;
    }
};

#endif //TARGET_TOOLS_INSTRUCTION_LOOKUP_H
