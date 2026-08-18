//
// Created by anolsen on 19.09.2019.
//

#include <cstdint>
#include "Size_adapter.h"

union buffer_union {
    uint32_t four_bytes;
    uint8_t single_byte[4];
};

void Size_adapter::store_byte(uint32_t *p_buffer, uint32_t index, uint8_t data) {
    uint32_t word_index = index >> 2u;
    uint32_t byte_index = 3 - (index & 3u);
    buffer_union item{};
    item.four_bytes = *(p_buffer + word_index);
    item.single_byte[byte_index] = data;
    *(p_buffer + word_index) = item.four_bytes;
}

uint8_t Size_adapter::fetch_byte(const uint32_t *p_buffer, uint32_t index, uint32_t write_flag) {
    uint32_t word_index = index >> 2u;
    uint32_t byte_index = 3 - (index & 3u);
    buffer_union item{};
    item.four_bytes = *(p_buffer + word_index);
    item.four_bytes |= write_flag;
    return item.single_byte[byte_index];
}
