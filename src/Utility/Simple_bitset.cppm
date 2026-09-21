//
// Created by Drift on 02.09.2020.
//

module;
#include <string.h>
#include <cstdint>

export module Utility.Simple_bitset;


export template<int N>
class Simple_bitset {
    enum {
        Size_of_buffer = (N + 7) / 8
    };
    uint8_t the_bits[Size_of_buffer]{};

public:
    void set_bit(uint32_t bit_number, bool raise) { raise ? raise_bit(bit_number) : clear_bit(bit_number); }

    void raise_bit(uint32_t bit_number) {
        uint32_t bit_index = bit_number % 8;
        uint32_t byte_index = bit_number / 8;
        if (byte_index < Size_of_buffer) {
            the_bits[byte_index] |= (1u << bit_index);
        }
    }

    void clear_bit(uint32_t bit_number) {
        uint32_t bit_index = bit_number % 8;
        uint32_t byte_index = bit_number / 8;
        if (byte_index < Size_of_buffer) {
            the_bits[byte_index] &= ~(1u << bit_index);
        }
    }

    bool test_bit(uint32_t bit_number) const {
        uint32_t bit_index = bit_number % 8;
        uint32_t byte_index = bit_number / 8;
        if (byte_index < Size_of_buffer) {
            return (the_bits[byte_index] & (1u << bit_index));
        } else {
            return false;
        }
    }

    void clear() {
        memset(the_bits, 0, Size_of_buffer);
    }

    bool is_clear() const {
        for (auto octet : the_bits) {
            if (octet != 0) return false;
        }
        return true;
    }
};
