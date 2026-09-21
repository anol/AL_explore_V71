//
// Created by anolsen on 15.01.2020.
//

module;
#include <cstdint>

export module Utility.Gray_code;

export namespace Gray_code {

    inline uint32_t gray_to_normal(uint32_t GrayVal) {
        uint32_t result = GrayVal;
        result ^= (result >> 1u);
        result ^= (result >> 2u);
        result ^= (result >> 4u);
        result ^= (result >> 8u);
        result ^= (result >> 16u);
        return result;
    }

}
