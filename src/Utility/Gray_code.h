//
// Created by anolsen on 15.01.2020.
//

#ifndef NORM_FW_GRAY_CODE_H
#define NORM_FW_GRAY_CODE_H

#include <cstdint>
namespace Gray_code {

    static uint32_t gray_to_normal(uint32_t GrayVal) {
        uint32_t result = GrayVal;
        result ^= (result >> 1u);
        result ^= (result >> 2u);
        result ^= (result >> 4u);
        result ^= (result >> 8u);
        result ^= (result >> 16u);
        return result;
    }

}
#endif //NORM_FW_GRAY_CODE_H
