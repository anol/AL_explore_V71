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
* @file   IDE3380_test_noise_floor.cpp
* @author AndersEmilOlsen, IDEAS
* @date   29.05.2026
* @brief  
*/

#include <cstdio>

#include "IDE3380_test_noise_floor.h"

#include <cstring>

#include "Instruction_major.h"
#include "Calibration_analyzer.h"
#include "Instruction_major.h"
#include "Configuration_repository.h"
#include "Persistent_parameter_id.h"

void HAL_Delay(int milliseconds);

uint32_t HAL_GetTick();

namespace Calibration {
    bool IDE3380_test_noise_floor::run_iterative() {
        switch (the_iteration) {
            case Stopped: {
                break;
            }
            case Starting: {
                print_attributes();
                main_prolog();
                the_iteration = First_channel;
                break;
            }
            case First_channel: {
                the_channel_index = 0;
                channel_prolog(the_channel_index);
                the_iteration = First_threshold;
                break;
            }
            case Next_channel: {
                channel_epilog(the_channel_index);
                the_channel_index++;
                if (the_channel_index < Number_of_channels) {
                    channel_prolog(the_channel_index);
                    the_iteration = First_threshold;
                } else {
                    the_iteration = Channels_done;
                }
                break;
            }
            case First_threshold: {
                the_threshold = the_start_threshold;
                threshold_prolog(the_channel_index, the_threshold);
                the_iteration = Begin_delay;
                break;
            }
            case Begin_delay: {
                the_wakeup_time = get_timestamp();
                the_wakeup_time += the_integration_time;
                the_iteration = Integration_delay;
                break;
            }
            case Integration_delay: {
                auto timestamp = get_timestamp();
                if (the_wakeup_time <= timestamp) {
                    the_iteration = Next_threshold;
                }
                break;
            }
            case Next_threshold: {
                auto count = threshold_epilog(the_threshold);
                if (count > the_stop_count) {
                    the_iteration = Next_channel;
                } else {
                    the_threshold--;
                    if (the_threshold > Min_threshold) {
                        threshold_prolog(the_channel_index, the_threshold);
                        the_iteration = Begin_delay;
                    } else {
                        the_iteration = Next_channel;
                    }
                }
                break;
            }
            case Channels_done: {
                main_epilog();
                the_iteration = Completed;
                break;
            }
            case Completed: {
                if (optional_instruction) {
                    optional_instruction->print_ack();
                }
                the_iteration = Stopped;
                break;
            }
            default:
            case Error_state: {
                main_epilog();
                if (optional_instruction) {
                    optional_instruction->print_nack("Test failed");
                }
                the_iteration = Stopped;
                break;
            }
        }
        return the_iteration != Stopped;
    }

    void IDE3380_test_noise_floor::print_diag() const {
        print_attributes();
        printf("CAL_TEST_NOISE: iteration=%d, channel=%d, threshold=%d\r\n",
               the_iteration, the_channel_index, the_threshold);
    }

    void IDE3380_test_noise_floor::set_attributes(
        const uint32_t integration_time, const uint8_t start_threshold, const int32_t stop_count) {
        the_integration_time = integration_time ? integration_time : Integration_time;
        the_start_threshold = start_threshold ? start_threshold : Start_threshold;
        the_stop_count = stop_count > 0 ? stop_count : Stop_count;
    }

    void IDE3380_test_noise_floor::print_attributes() const {
        printf("Noise floor calibration test: integration_time=%d, start_threshold=%d, stop_count=%d",
               the_integration_time, the_start_threshold, the_stop_count);
        if (is_trace()) {
            printf("\r\n");
        } else {
            printf(", ");
        }
    }

    bool IDE3380_test_noise_floor::start_test(Instruction_major *instruction) {
        optional_instruction = instruction;
        the_iteration = Starting;
        return true;
    }

    void IDE3380_test_noise_floor::main_prolog() const {
        use_IDE3380.disable_all_channels();
    }

    void IDE3380_test_noise_floor::main_epilog() {
        update_configuration();
        print_results();
    }

    void IDE3380_test_noise_floor::update_configuration() const {
        use_IDE3380.restore_all_channels();
        for (uint8_t index = 0; index < Number_of_channels; index++) {
            auto reg_value = use_IDE3380.set_channel_threshold(index, the_noise_floor[index]);
            auto id = Application::IDE3380_0 + index;
            use_repository.set(id, static_cast<int32_t>(reg_value));
        }
    }

    void IDE3380_test_noise_floor::print_results() {
        printf("\r\n");
        printf("Channel:     ");
        for (uint8_t index = 0; index < Number_of_channels; index++) {
            printf(" %3d", index + 1);
        }
        printf("\r\n");
        printf("Noise floor: ");
        for (unsigned char value: the_noise_floor) {
            printf(" %3d", value);
        }
        printf("\r\n");
    }

    void IDE3380_test_noise_floor::channel_prolog(const uint8_t index) {
        if (is_trace()) printf("Ch %d:", index + 1);
        else printf(".");
        memset(the_integration_count, 0, sizeof(the_integration_count));
        use_IDE3380.enable_channel(index);
    }

    void IDE3380_test_noise_floor::channel_epilog(const uint8_t index) {
        use_IDE3380.disable_channel(index);
        the_noise_floor[index] = Calibration_analyzer::find_noise_floor_A(the_integration_count);
        if (is_trace()) printf("(%d)\r\n\r\n", the_noise_floor[index]);
    }

    void IDE3380_test_noise_floor::threshold_prolog(const uint8_t index, const uint8_t threshold) {
        use_IDE3380.set_channel_threshold(index, threshold);
        if (is_trace()) printf(" %d=", threshold);
        use_event_counter.reset_event_count();
    }

    int32_t IDE3380_test_noise_floor::threshold_epilog(const uint8_t threshold) {
        const auto count = static_cast<int32_t>(use_event_counter.reset_event_count());
        if (is_trace()) printf("%d,", count);
        the_integration_count[threshold] = count;
        return count;
    }

    uint32_t IDE3380_test_noise_floor::get_timestamp() {
        return 0; // HAL_GetTick();
    }
} // IDE3380

namespace Calibration {
    // Please note: the run_linear is used for unit testing only, to compare results vs. the run_iterative.
    bool IDE3380_test_noise_floor::run_linear() {
        the_integration_time = Integration_time;
        main_prolog();
        for (uint8_t index = 0; index < Number_of_channels; index++) {
            channel_prolog(index);
            for (uint8_t threshold = the_start_threshold; threshold > 0; threshold--) {
                threshold_prolog(index, threshold);
                blocking_integration_delay(the_integration_time);
                const auto count = threshold_epilog(threshold);
                if (count > the_stop_count) {
                    break;
                }
            }
            channel_epilog(index);
        }
        main_epilog();
        return false;
    }

    void IDE3380_test_noise_floor::blocking_integration_delay(const uint32_t milliseconds) {
        HAL_Delay(milliseconds);
    }
}
