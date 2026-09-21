//
// Created by Drift on 29.09.2020.
//

module;
#include <cstdio>
#include <cstdint>

module Support.Instruction_major;
import Platform.Diagnostic;

using namespace Instruction;
using namespace Error_handling;

Instruction_major::Instruction_major(const keyset_t &keys, const uint8_t literal_key, const Instruction_token &token) {
    build_instruction(keys, literal_key);
    add_token(token);
    if (the_token_count > 0) {
        close();
    }
}

Instruction_major::Instruction_major(const keyset_t &keys, const uint8_t literal_key) {
    build_instruction(keys, literal_key);
    if (the_token_count > 0) {
        close();
    }
}

void Instruction_major::build_instruction(const uint8_t (&keys)[16], const uint8_t literal_key) {
    bool next_is_literal{};
    uint8_t value_index{};
    for (uint8_t index = 0; index < Max_tokens; index++) {
        auto key = keys[index];
        if (next_is_literal) {
            next_is_literal = false;
            if (value_index) {
                the_token_values[value_index] = key;
                the_token_types[value_index] = Integer_token;
            }
        } else if (literal_key == key) {
            the_token_ids[index] = 0;
            the_token_values[index] = 0;
            the_token_types[index] = End_token;
            if (index > 1) {
                next_is_literal = true;
                value_index = index - 1;
            } else {
                break;
            }
        } else {
            the_token_ids[index] = key;
            the_token_values[index] = 0;
            the_token_types[index] = Keyword_token;
            if (key > 0) {
                the_token_count++;
            } else {
                break;
            }
        }
    }
}

void Instruction_major::add_token(const Instruction_token &token) {
    if (the_token_count < (Max_tokens - 1)) {
        the_token_ids[the_token_count] = 0xFFu & token.the_id;
        the_token_values[the_token_count] = token.the_value;
        the_token_types[the_token_count] = token.token_type;
        if (token.token_type == String_token) {
            token.unpack_string(the_string_buffer, String_buffer_size);
        }
        the_token_count++;
    }
}

uint32_t Instruction_major::find_token_value(uint32_t id) {
    for (uint32_t index = the_token_count - 1; index > 0; index--) {
        if (id == the_token_ids[index]) {
            return the_token_values[index];
        }
    }
    return 0;
}

void Instruction_major::set_request(uint32_t tag) {
    the_command_id = 1; // NOTE: Use "1 == Special command" for internal built instructions
    the_token_ids[0] = 0xFFu & tag;
    the_token_values[0] = tag;
    the_token_types[0] = Keyword_token;
    the_token_count = 1;
}

uint32_t Instruction_major::add_command(uint32_t tag) {
    uint32_t index = the_token_count;
    if (index < (Max_tokens - 1)) {
        the_token_ids[index] = 0xFFu & tag;
        the_token_values[index] = 0;
        the_token_types[index] = Keyword_token;
        the_token_count++;
    }
    return index;
}

uint32_t Instruction_major::add_value(uint32_t tag, uint32_t value, Token_type token_type) {
    uint32_t index = the_token_count;
    if (index < (Max_tokens - 1)) {
        the_token_ids[index] = 0xFFu & tag;
        the_token_values[index] = value;
        the_token_types[index] = token_type;
        the_token_count++;
    }
    return index;
}

uint32_t Instruction_major::add_string(uint32_t tag, const char *string) {
    uint32_t index = the_token_count;
    if (index < (Max_tokens - 1)) {
        Instruction_token token{String_token, tag, nullptr, 0, 0};
        token.pack_string(string);
        add_token(token);
    }
    return index;
}


uint32_t Instruction_major::add_string(uint32_t tag, const char *string, size_t n) {
    uint32_t index = the_token_count;
    if (index < (Max_tokens - 1)) {
        Instruction_token token{String_token, tag, nullptr, 0, 0};
        token.pack_string(string, n);
        add_token(token);
    }
    return index;
}


void Instruction_major::set_string(uint32_t packed) {
    uint32_t index = 0;
    while (packed && index < String_buffer_size) {
        the_string_buffer[index++] = static_cast<char>(packed & 0xFFu);
        packed >>= 8u;
    }
    the_string_buffer[index] = 0;
}

void Instruction_major::set_keyword(uint32_t index, uint32_t tag) {
    if (index < the_token_count) {
        the_token_ids[index] = 0xFFu & tag;
        the_token_types[index] = Keyword_token;
    }
}

//const char *Instruction_major::get_token_string(uint32_t index, char *buffer, uint32_t max_size) const {
//    if (index < the_token_count) {
//        uint8_t start_id = the_token_ids[index];
//        char *position = buffer;
//        while ((index < the_token_count) && (start_id == the_token_ids[index]) && (--max_size > 0)) {
//            *position++ = 0xFFu & (the_token_values[index] >> 8);
//            *position++ = 0xFFu & the_token_values[index];
//            index++;
//        }
//        *position = 0;
//        return buffer;
//    } else {
//        return nullptr;
//    }
//}

void Instruction_major::print_trace(const Keyword_lookup get_keyword) const {
    for (uint32_t index = 0; index < the_token_count; index++) {
        if (index > 0) printf(", ");
        if (the_token_types[index] == Keyword_token) {
            printf("%s", get_keyword(the_token_ids[index]));
        } else if (the_token_types[index] == String_token) {
            printf("%s=%s", get_keyword(the_token_ids[index]), get_string());
        } else {
            printf("%s=%d", get_keyword(the_token_ids[index]), static_cast<int>(the_token_values[index]));
        }
    }
    printf("\r\n");
}

// void Instruction_major::report_trace(Service_report *reporter) const {
//     if (reporter) {
//         reporter->begin_object();
//         for (uint32_t index = 0; index < the_token_count; index++) {
//             if (the_token_types[index] == Keyword_token) {
//                 reporter->keyword(the_token_ids[index]);
//             } else {
//                 reporter->report(the_token_ids[index], the_token_values[index]);
//             }
//         }
//         reporter->end_object();
//     }
// }

// void Instruction_major::report(Service_report *reporter) const {
//     if (reporter) {
//         reporter->begin_object();
//         reporter->report(SpectraNode_interface::Key_STATUS, (uint32_t) reporter->get_transaction_id());
//         for (uint32_t index = 0; index < the_token_count; index++) {
//             if (the_token_types[index] == Keyword_token) {
//                 reporter->keyword(the_token_ids[index]);
//             } else if (the_token_types[index] == String_token) {
//                 reporter->report(the_token_ids[index], get_string());
//             } else {
//                 reporter->report(the_token_ids[index], the_token_values[index]);
//             }
//         }
//         reporter->end_object();
//     }
// }

void Instruction_major::missing_data() {
    error_message("MISSING DATA.");
}

void Instruction_major::missing_argument() {
    error_message("MISSING ARGUMENT(S)");
}

void Instruction_major::value_out_of_range() {
    error_message("VALUE OUT OF RANGE");
}

void Instruction_major::unknown_argument(const char *word) {
    if (word) {
        error_message("'%s' IS UNKNOWN", word);
    } else {
        error_message("UNKNOWN ARGUMENT");
    }
}

void Instruction_major::ambiguous_argument(const char *word) {
    if (word) {
        error_message("'%s' IS AMBIGUOUS", word);
    } else {
        error_message("AMBIGUOUS ARGUMENT", word);
    }
}

void Instruction_major::oversized_string(const char *word, uint32_t actual, uint32_t limit) {
    error_message("STRING SIZE %lu IS LARGER THAN %lu", actual, limit);
}

void Instruction_major::print_pending() {
    printf("PENDING\r\n");
}

void Instruction_major::print_ack() {
    printf("OK\r\n");
    clean();
}

void Instruction_major::print_ack(const int32_t data) {
    printf("OK=%d\r\n", static_cast<int>(data));
    clean();
}

void Instruction_major::print_ack(const uint32_t reg, const uint32_t data) {
    printf("OK=%d,%d\r\n", static_cast<int>(reg), static_cast<int>(data));
    clean();
}

void Instruction_major::print_nack(const uint32_t error) {
    printf("ERROR=%d\r\n", static_cast<int>(error));
    clean();
}

void Instruction_major::print_nack(const char* error) {
    printf("ERROR=%s\r\n", error);
    clean();
}
