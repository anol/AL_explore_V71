/*
* Copyright (C) 2026 Integrated Detector Electronics AS
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
* @file   IDE3380_test_noise_floor.h
* @author AndersEmilOlsen, IDEAS
* @date   29.05.2026
* @brief
*/


#pragma once

import Type.Abstract_scenario;

#include "IDE3380_interface.h"
#include "Event_counter.h"
import Support.Instruction_major;


using namespace IDE3380;

namespace Calibration {
    class IDE3380_test_noise_floor : public Abstract::Abstract_scenario<Instruction_major> {
        enum {
            Number_of_channels = 16,
            Min_threshold = 0, Max_threshold = 254, Number_of_thresholds = 256,
            Integration_time = 200,
            Start_threshold = 99,
            Stop_count = 20,
        };

        enum Iteration_state {
            Stopped, Starting, First_channel, Next_channel, First_threshold, Next_threshold, Begin_delay,
            Integration_delay,
            Channels_done, Completed, Error_state,
        };

        IDE3380_register_access &use_IDE3380;
        Application::Event_counter &use_event_counter;
        Repository::Configuration_repository &use_repository;
        Iteration_state the_iteration{};
        uint32_t the_integration_time{Integration_time};
        int32_t the_stop_count{Stop_count};
        uint8_t the_start_threshold{Start_threshold};
        uint8_t the_threshold{};
        uint8_t the_channel_index{};
        uint32_t the_wakeup_time{};
        int32_t the_integration_count[Number_of_thresholds]{};
        uint8_t the_noise_floor[Number_of_channels]{};
        Instruction_major *optional_instruction{};

    public:
        IDE3380_test_noise_floor(IDE3380_interface &IDE3380, Application::Event_counter &event_counter,
                                 Repository::Configuration_repository &repository)
            : use_IDE3380(IDE3380.get_register_access()), use_event_counter(event_counter), use_repository(repository) {
        }

        [[nodiscard]] bool is_active() const override { return Stopped != the_iteration; }

        void set_attributes(uint32_t integration_time = Integration_time,
                            uint8_t start_threshold = Start_threshold,
                            int32_t stop_count = Stop_count);

        [[nodiscard]] Status_code start_test(Instruction_major *instruction) override;

        void background_process() override {
            run_iterative();
        }

        void print_diag() const override;

        bool run_iterative();

        bool run_linear();

    private:
        void print_attributes() const;

        void main_prolog() const;

        void update_configuration() const;

        void print_results();

        void main_epilog();

        void channel_prolog(uint8_t index);

        void channel_epilog(uint8_t index);

        void threshold_prolog(uint8_t index, uint8_t threshold);

        int32_t threshold_epilog(uint8_t threshold);

        static uint32_t get_timestamp();

        static void blocking_integration_delay(uint32_t milliseconds);
    };
} // IDE3380
