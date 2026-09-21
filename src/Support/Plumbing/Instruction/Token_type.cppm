//
// Created by AndersEmilOlsen on 10.01.2024.
//

module;
#include <cstdint>

export module Support.Token_type;

export namespace Instruction {

    using float32_t = float;

    enum Token_type : uint8_t {
        End_token = 0,
        Command_token,
        Keyword_token,
        Integer_token,
        Float_token,
        Bitmask_token,
        String_token,
        Question_mark,
        Help_text,
    };

    enum Command_id : uint8_t {
        No_such_command, Special_command,
    };


} // Instruction
