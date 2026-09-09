//
// Created by Drift on 29.09.2020.
//

#ifndef TARGET_WINDOWS_INSTRUCTION_MAJOR_H
#define TARGET_WINDOWS_INSTRUCTION_MAJOR_H

#include <cstring>
#include "Dictionary/Dictionary.h"

using namespace Instruction;

class Instruction_major {
public:
    enum {
        Max_tokens = 16,
        String_buffer_size = Instruction_token::Max_string_length + 1
    };

    typedef uint8_t keyset_t[Max_tokens];

private:
    union {
        uint32_t the_token_values[Max_tokens]{};
        int32_t the_integer_values[Max_tokens];
        float32_t the_float_values[Max_tokens];
    };

    uint32_t the_token_count{};
    Token_type the_token_types[Max_tokens]{};
    uint8_t the_token_ids[Max_tokens]{};
    char the_string_buffer[String_buffer_size]{};
    uint32_t the_command_id{};
    bool the_AT_mode_flag{};

public:
    Instruction_major() = default;

    /// The purpose of this constructor is to allow for array initialization scheduled activities.
    explicit Instruction_major(const keyset_t &keys);

    /// This constructor is used when you need a single argument.
    Instruction_major(const keyset_t &keys, const Instruction_token &);

    void clean() {
        memset(the_token_ids, 0, sizeof(the_token_ids));
        the_token_count = 0;
        the_command_id = 0;
    }

    [[nodiscard]] bool is_clean() const { return the_token_count == 0; }

    [[nodiscard]] uint32_t get_command_id() const { return the_command_id; }

    void close(uint32_t id = Wildcard) { the_command_id = id; }

    void add_token(const Instruction_token &token);

    uint8_t *get_token_ids(uint8_t &size) const {
        size = the_token_count;
        return const_cast<uint8_t *>(the_token_ids);
    }

    [[nodiscard]] uint8_t get_token_id(uint32_t index) const {
        return (index < the_token_count) ? the_token_ids[index] : 0;
    }

    [[nodiscard]] Token_type get_token_type(uint32_t index) const {
        return (index < the_token_count) ? the_token_types[index] : End_token;
    }

    [[nodiscard]] int32_t get_token_integer(uint32_t index) const {
        if (index < the_token_count) return the_integer_values[index];
        return 0;
    }

    [[nodiscard]] float32_t get_token_float(uint32_t index) const {
        if (index < the_token_count) return the_float_values[index];
        return 0;
    }

    [[nodiscard]] uint32_t get_number_of_tokens() const { return the_token_count; }

    [[nodiscard]] const char *get_string() const { return the_string_buffer; }

    void set_AT_mode(const bool mode) { the_AT_mode_flag = mode; }

    [[nodiscard]] bool is_AT_mode() const { return the_AT_mode_flag; }

    void missing_data();

    void missing_argument();

    void value_out_of_range();

    void unknown_argument(const char *);

    void ambiguous_argument(const char *);

    void oversized_string(const char *, uint32_t actual, uint32_t limit);

    void print_pending();

public:
    [[nodiscard]] uint32_t get_id() const { return *((uint32_t *) the_token_ids); }

    void print_ack();

    void print_ack(int32_t data);

    void print_ack(uint32_t reg, uint32_t data);

    void print_nack(uint32_t error);

    void print_nack(const char *error);

    [[nodiscard]] const uint8_t *get_identification_string() const { return the_token_ids; }

    void set_token_id(uint32_t index, uint32_t id) {
        if (index < the_token_count) the_token_ids[index] = id;
    }

    void set_value(uint32_t index, uint32_t value) {
        if (index < the_token_count) the_token_values[index] = value;
    }

    void set_token_type(uint32_t index, Token_type type) {
        if (index < the_token_count) the_token_types[index] = type;
    }

    uint32_t find_token_value(uint32_t id);

    [[nodiscard]] bool is_valid() const { return the_command_id > 0 && the_token_count > 0; }

    void print_trace() const;

    void set_request(uint32_t tag);

    uint32_t add_command(uint32_t tag);

    uint32_t add_value(uint32_t tag, uint32_t value, Token_type token_type = Integer_token);

    uint32_t add_string(uint32_t tag, const char *string);

    uint32_t add_string(uint32_t tag, const char *string, size_t n);

    void set_keyword(uint32_t index, uint32_t tag);

    void set_string(uint32_t packed);

    void build_instruction(const uint8_t (&keys)[16]);
};


#endif //TARGET_WINDOWS_INSTRUCTION_MAJOR_H
