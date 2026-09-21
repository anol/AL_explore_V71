/*
* Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
* @file   IDE3380_test_pedestal.cpp
* @author AndersEmilOlsen, IDEAS
* @date   12.05.2026
* @brief  
*/

#include <cstdio>

#include "IDE3380_test_pedestal.h"

#include "Configuration_repository.h"
#include "Instruction_major.h"

namespace Calibration {
    void IDE3380_test_pedestal::test_prolog(const int32_t count) {
        the_readout_countdown = count;
        use_register_access.override_all_thresholds();
        use_io_control.enable_external_hold();
        use_readout_control.set_ignore_trigger(true);
        use_histogram.clear_histogram_buffer();
        use_histogram.clear_channel_pedestal();
        use_io_control.raise_external_hold();
        use_io_control.start_readout();
    }

    bool IDE3380_test_pedestal::test_loop() const {
        const auto readout_done{use_io_control.continue_readout()};
        if (readout_done) {
            if (use_io_control.is_external_hold()) {
                use_io_control.cease_external_hold();
            } else {
                use_io_control.raise_external_hold();
                use_io_control.start_readout();
            }
        }
        return readout_done;
    }

    void IDE3380_test_pedestal::test_epilog() {
        use_io_control.disable_external_hold();
        use_register_access.restore_all_channels();
        use_readout_control.set_ignore_trigger(false);
        test_result();
        update_settings();
        the_readout_countdown = false;
    }

    void IDE3380_test_pedestal::test_result() {
        printf("Chn.: ");
        for (int channel = 0; channel < IDE3380_input_count; channel++) {
            printf("%3d ", channel);
            the_pedestals[channel] = weighted_mean_index(use_histogram.get_channel_buffer(channel), IDE3380_ADC_range);
        }
        printf("\r\n");
        printf("Mean: ");
        for (auto pedestal: the_pedestals) {
            printf("%3d ", pedestal);
        }
        if (optional_instruction) {
            optional_instruction->print_ack();
        }
        use_histogram.clear_histogram_buffer();
    }

    void IDE3380_test_pedestal::update_settings() {
        for (int32_t channel = 0; channel < IDE3380_input_count; channel++) {
            auto id = Application::Cal_input_1_offset + channel;
            use_repository.set(id, the_pedestals[channel]);
        }
        use_histogram.update_pedestals(use_repository, Application::Cal_input_1_offset);
    }

    void IDE3380_test_pedestal::print_diag() const {
        printf("CAL_TEST_PEDESTAL: countdown=%d, iterator=%d\r\n",
               the_readout_countdown, use_readout_control.get_TXD_iteration());
    }

    int32_t IDE3380_test_pedestal::weighted_mean_index(const uint32_t *data, const uint32_t size) {
        uint32_t weighted_sum = 0.0;
        uint32_t total_weight = 0.0;
        for (uint32_t i = 0; i < size; ++i) {
            weighted_sum += i * data[i];
            total_weight += data[i];
        }
        if (total_weight == 0.0) {
            return 0; // undefined if all weights are zero
        }
        return static_cast<int32_t>(weighted_sum / total_weight);
    }
} // IDE3380
