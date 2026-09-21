/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 */
/**
 * \date   IDEAS/21.06.2021/aeols
 * \brief
 */

module;
#include <cstdint>

export module Utility.Histogram;
import Type.Misc_type;

export template<uint32_t Number_of_bins>
class Histogram {
    uint32_t the_min{};
    uint32_t the_max{};
    uint32_t the_bin_span{};
    uint32_t the_bin_count{};
    uint16_t the_bins[Number_of_bins]{};

    uint32_t to_bin(uint32_t value) const {
        uint32_t bin = 0;
        if ((the_bin_span > 0) && (the_bin_count > 0)) {
            if (value >= the_max) {
                bin = the_bin_count - 1;
            } else if (value >= the_min) {
                bin = (value - the_min) / the_bin_span;
            }
        }
        return bin;
    }

public:
    bool initialize(uint32_t bin_min, uint32_t bin_count, uint32_t bin_span) {
        the_min = 0;
        the_max = 0;
        the_bin_span = 0;
        the_bin_count = 0;
        memset(the_bins, 0, sizeof(the_bins));
        if ((0 < bin_count) && (bin_count <= Number_of_bins) && (0 < bin_span)) {
            the_min = bin_min;
            the_max = bin_min + (bin_count - 1) * bin_span;
            the_bin_span = bin_span;
            the_bin_count = bin_count;
        }
        return (the_min < the_max);
    }

    void count_hit(uint32_t value) {
        auto bin = to_bin(value);
        if ((bin < Number_of_bins) && (the_bins[bin] < 0xFFF0)) the_bins[bin]++;
    }

    uint16_t get_hits(uint32_t bin) const {
        if (bin < Number_of_bins) return the_bins[bin]; else return 0;
    }

    void for_each_bin(Optional_user user, void (*func)(Optional_user, uint32_t bin, uint32_t hits)) const {
        if (func) {
            for (uint32_t bin = 0; bin < Number_of_bins; bin++) {
                auto hits = the_bins[bin];
                if (0 < hits) {
                    func(user, bin, hits);
                }
            }
        }
    }

    uint32_t get_start_bin() const { return the_min; }

    uint32_t get_stop_bin() const { return the_max; }

    uint32_t get_bin_span() const { return the_bin_span; }

    uint32_t get_bin_count() const { return the_bin_count; }

};
