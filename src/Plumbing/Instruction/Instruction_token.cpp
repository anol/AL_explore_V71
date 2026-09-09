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

#include <cstring>

#include "Instruction_token.h"

using namespace Instruction;

Instruction_token::Instruction_token() : token_type(End_token) {}

Instruction_token::Instruction_token(uint8_t id) :
        token_type(Keyword_token), the_id(id) {}

Instruction_token::Instruction_token(uint8_t id, uint32_t value) :
        token_type(Integer_token),
        the_id(id),
        the_value(value) {}

Instruction_token::Instruction_token(uint8_t id, const char *string) :
        token_type(String_token),
        the_id(id) {
    pack_string(string);
}

Instruction_token::Instruction_token(const Instruction_token *p_token) :
        token_type(p_token->token_type),
        the_id(p_token->the_id),
        optional_keyword(p_token->optional_keyword) {}

Instruction_token::Instruction_token(const Instruction_token *p_token, int32_t value) :
        token_type(p_token->token_type),
        the_id(p_token->the_id),
        the_integer_value(value),
        optional_keyword(p_token->optional_keyword) {}

Instruction_token::Instruction_token(const Instruction_token *p_token, float32_t value) :
        token_type(p_token->token_type),
        the_id(p_token->the_id),
        the_float_value(value),
        optional_keyword(p_token->optional_keyword) {}

Instruction_token::Instruction_token(const Instruction_token *p_token, const char *string) :
        token_type(p_token->token_type),
        the_id(p_token->the_id) {
    if (p_token->token_type == Token_type::String_token) {
        pack_string(string);
    }
}

void Instruction_token::clear() {
    token_type = End_token;
    optional_keyword = "";
    the_id = 0;
    the_value = 0;
    optional_next = nullptr;
}

void Instruction_token::pack_string(const char *string) {
    if (string != nullptr)
    {
        pack_string(string, strlen(string));
    }
}


void Instruction_token::pack_string(const char *string, uint32_t length)
{
    if (length <= 12u)
    {
        while (length-- > 0u)
        {
            uint8_t sym = *string++;
            the_max_value <<= 8u;
            the_max_value |= 0xFFu & (the_min_value >> 24u);
            the_min_value <<= 8u;
            the_min_value |= 0xFFu & (the_value >> 24u);
            the_value <<= 8u;
            the_value |= sym;
        }
    }
}

bool Instruction_token::unpack_string(char *string, uint32_t length) const {
    bool result = false;
    char sym;
    if (length > Max_packet_length) {
        sym = (char) (0xFFu & (the_max_value >> 24u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_max_value >> 16u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_max_value >> 8u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & the_max_value);
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_min_value >> 24u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_min_value >> 16u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_min_value >> 8u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & the_min_value);
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_value >> 24u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_value >> 16u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & (the_value >> 8u));
        if (sym != 0) *string++ = sym;
        sym = (char) (0xFFu & the_value);
        if (sym != 0) *string++ = sym;
        *string = 0;
        result = true;
    }
    return result;
}

