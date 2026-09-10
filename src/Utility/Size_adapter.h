//
// Created by anolsen on 19.09.2019.
//

#ifndef UTILITY_SIZE_ADAPTER_H
#define UTILITY_SIZE_ADAPTER_H

#include "../Type/Misc_type.h"

namespace Size_adapter {
    void store_byte(uint32_t *p_buffer, uint32_t byte_index, uint8_t data);

    uint8_t fetch_byte(const uint32_t *p_buffer, uint32_t byte_index, uint32_t write_flag = 0);

    inline uint32_t swap_shorts(uint32_t data) {
        return ((0xFFFF & data) << 16) | (0xFFFF & (data >> 16));
    }

    inline uint32_t swap_endian(uint32_t data) {
        return ((0x00FF & data) << 24) |
               ((0xFF00 & data) << 8) |
               (0xFF00 & (data >> 8)) |
               (0x00FF & (data >> 24));
    }
}

#endif //UTILITY_SIZE_ADAPTER_H
