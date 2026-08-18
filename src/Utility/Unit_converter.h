//
// Created by anolsen on 11.05.2020.
//

#ifndef TARGET_EVAL_V71_UNIT_CONVERTER_H
#define TARGET_EVAL_V71_UNIT_CONVERTER_H

#include <cstdint>

namespace Unit_converter {

    static uint32_t nanoseconds_to_count(uint32_t frequency_Hz, uint32_t width_ns) {
        uint32_t result = 0;
        if ((frequency_Hz > 0) && (width_ns > 0)) {
            double width_in_seconds = ((double) width_ns) / 1.0E9;
            double pulse_width = 1.0 / ((double) frequency_Hz);
            double number_of_pulses = 0.1 + width_in_seconds / pulse_width;
            result = number_of_pulses;
        }
        return result;
    }

}


#endif //TARGET_EVAL_V71_UNIT_CONVERTER_H
