//
// Created by anolsen on 11.05.2020.
//

module;
#include <cmath>
#include <cstdint>

export module Utility.Unit_converter;

export namespace Unit_converter {
    inline uint32_t nanoseconds_to_count(const uint32_t frequency_Hz, const uint32_t width_ns) {
        uint32_t result = 0;
        if ((frequency_Hz > 0) && (width_ns > 0)) {
            const double width_in_seconds = static_cast<double>(width_ns) / 1.0E9;
            const double pulse_width = 1.0 / static_cast<double>(frequency_Hz);
            const double number_of_pulses = 0.1 + width_in_seconds / pulse_width;
            result = floor(number_of_pulses);
        }
        return result;
    }

    /// @brief A temperature split into parts suitable for printing with "%s%d.%02d".
    struct Celsius_parts {
        const char *sign;   ///< "-" for negative temperatures, otherwise "".
        int32_t degrees;    ///< Whole degrees, never negative.
        int32_t hundredths; ///< Hundredths of a degree, 0..99.
    };

    /// @brief Split a temperature in centidegrees Celsius (1/100 C) into sign, degrees and hundredths.
    /// The sign is kept separately so that e.g. -50 (-0.50C) does not print as "0.50".
    inline Celsius_parts centidegrees_to_parts(const int32_t centidegrees) {
        const bool negative = centidegrees < 0;
        const int32_t magnitude = negative ? -centidegrees : centidegrees;
        return {.sign = negative ? "-" : "", .degrees = magnitude / 100, .hundredths = magnitude % 100};
    }
}
