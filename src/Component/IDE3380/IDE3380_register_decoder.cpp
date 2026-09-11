/*
* Copyright (C) 2025-2026 Integrated Detector Electronics AS
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
* @file   IDE3380_register_decoder.cpp
* @author AndersEmilOlsen, IDEAS
* @date   17.03.2026
* @brief  
*/


#include "IDE3380_register_decoder.h"

#include <cstdio>

namespace IDE3380 {
    void IDE3380_register_decoder::dump(uint32_t address, uint32_t value) {
        enum { Channel_count = 17 };
        the_value = value;
        printf(" 0x02X\r\n", address);
        if (address == 0) {
            dump_field("cmis_detector_voffset", 18, 8, "AIN input voltage offset");
            dump_field("cmis_detector_ioffset", 15, 3, "AIN input current offset");
            dump_field("cmis_impedance_reduction", 14, 1, "AIN input impedance (VBIAS. 0=5uA, 1=10uA)");
            dump_field("cal_select_channel", 13, 1, "Calibration Test Enable");
            dump_field("qc_threshold", 5, 8, "Charge comparator threshold");
            dump_field("qc_hysteresis", 2, 3, "Charge comparator hysteresis");
            dump_field("pu_channel", 1, 1, "Power up channel");
            dump_field("enable_triggering", 0, 1, "Enable channel trigger outputs");
        } else if (address < Channel_count) {
            dump_field("cmis_detector_voffset", 18, 8);
            dump_field("cmis_detector_ioffset", 15, 3);
            dump_field("cmis_impedance_reduction", 14, 1);
            dump_field("cal_select_channel", 13, 1);
            dump_field("qc_threshold", 5, 8);
            dump_field("qc_hysteresis", 2, 3);
            dump_field("pu_channel", 1, 1);
            dump_field("enable_triggering", 0, 1);
        }
    }

    void IDE3380_register_decoder::dump_field(const char *name, int pos, int width, const char *brief) const {
        printf("  %s=%d", name, field_value(pos, width));
        if (brief) {
            printf(" (%d:%d, %s)", pos, width, brief);
        }
        printf("\r\n");
    }

    int IDE3380_register_decoder::field_value(int pos, int width) const {
        auto value = the_value >> pos;
        return static_cast<int>(value & ((1 << width) - 1));
    }

    int IDE3380_register_decoder::field_value(const uint32_t value, const int pos, const int width) {
        return static_cast<int>((value >> pos) & ((1 << width) - 1));
    }

    void IDE3380_register_decoder::dump_decoded(const uint32_t *data, uint32_t count) {
        if (count > IDE3380_channel_count) {
            count = IDE3380_channel_count;
        }
        printf("            ");
        for (uint32_t gpc = 0; gpc < count; gpc++) {
            printf(" %3d", gpc);
        }
        printf("\r\n");
        dump_decoded("cmis_voffs", 18, 8, data, count);
        dump_decoded("cmis_ioffs", 15, 3, data, count);
        dump_decoded("cmis_im_re", 14, 1, data, count);
        dump_decoded("cal_sel_ch", 13, 1, data, count);
        dump_decoded("qc_threshl", 5, 8, data, count);
        dump_decoded("qc_hystere", 2, 3, data, count);
        dump_decoded("pu_channel", 1, 1, data, count);
        dump_decoded("en_trigger", 0, 1, data, count);
    }

    void IDE3380_register_decoder::dump_decoded(const char *name, const int pos, const int width,
                                                const uint32_t *data, const uint32_t count) {
        printf("%s: ", name);
        for (uint32_t gpc = 0; gpc < count; gpc++) {
            printf(" %3d", field_value(*data++, pos, width));
        }
        printf("\r\n");
    }
} // Application
