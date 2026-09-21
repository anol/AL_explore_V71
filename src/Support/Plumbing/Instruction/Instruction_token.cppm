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
// Created by anolsen on 20.02.2020.
//

module;
#include <cstdint>

export module Support.Instruction_token;
export import Support.Token_type;
export import Type.Status_code;

export namespace Instruction {

    struct Instruction_token {
        enum {
            Max_packet_length = 12,
            Max_string_length = 32
        };

        Token_type token_type{};
        uint16_t the_id{};
        union {
            uint32_t the_value{};
            int32_t the_integer_value;
            float32_t the_float_value;
        };
        int32_t the_min_value{};
        int32_t the_max_value{};
        const Instruction_token *optional_next{};
        const char *optional_keyword{""};

        Instruction_token();

        explicit Instruction_token(uint8_t id);

        Instruction_token(uint8_t id, uint32_t value);

        Instruction_token(uint8_t id, const char *string);

        explicit Instruction_token(const Instruction_token *p_token);

        Instruction_token(const Instruction_token *p_token, int32_t value);

        Instruction_token(const Instruction_token *p_token, float32_t value);

        Instruction_token(const Instruction_token *p_token, const char *string);

        constexpr
        Instruction_token(const char *help_text, uint32_t command, uint8_t provider) :
                token_type(Token_type::Help_text),
                the_id(provider),
                the_value(command),
                optional_keyword(help_text) {}

        constexpr
        Instruction_token(Token_type type, const char *string, uint32_t id, int32_t value,
                          const Instruction_token *next) :
                token_type(type), the_id(0xFF & id), the_integer_value(value), optional_next(next),
                optional_keyword(string) {}

        constexpr
        Instruction_token(Token_type type, const char *string, uint32_t id, uint32_t value,
                          const Instruction_token *next) :
                token_type(type), the_id(0xFF & id), the_value(value), optional_next(next), optional_keyword(string) {}

        constexpr
        Instruction_token(Token_type type, const char *string, uint32_t id, float32_t value,
                          const Instruction_token *next) :
                token_type(type), the_id(0xFF & id), the_float_value(value), optional_next(next),
                optional_keyword(string) {}

        constexpr
        Instruction_token(Token_type type, uint32_t id, const Instruction_token *next, int min, int max) :
                token_type(type), the_id(0xFF & id), the_min_value(min), the_max_value(max), optional_next(next) {}

        void clear();

        Status_code unpack_string(char *string, uint32_t length) const;

        void pack_string(const char *string);

        void pack_string(const char *string, uint32_t length);

    };

}
