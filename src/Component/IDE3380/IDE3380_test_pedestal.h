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
* @file   IDE3380_test_pedestal.h
* @author AndersEmilOlsen, IDEAS
* @date   12.05.2026
* @brief
*/


#pragma once

import Type.Abstract_scenario;

#include "Histogram_storage.h"
#include "IDE3380_interface.h"
import Support.Instruction_major;


using namespace IDE3380;

namespace Calibration {
    class IDE3380_test_pedestal : public Abstract::Abstract_scenario<Instruction_major> {
        enum {
            Default_diag_code = 76,
            Readout_count = 100'000,
        };

        IDE3380_interface &use_io_control;
        IDE3380_register_access &use_register_access;
        IDE3380_readout_control &use_readout_control;
        Application::Histogram_storage &use_histogram;
        Repository::Configuration_repository &use_repository;
        Instruction_major *optional_instruction{};
        int32_t the_pedestals[IDE3380_input_count]{};
        int32_t the_readout_countdown{0};
        uint32_t the_diag_code{Default_diag_code};

    public:
        IDE3380_test_pedestal(IDE3380_interface &ASIC, Application::Histogram_storage &histogram,
                              Repository::Configuration_repository &repository)
            : use_io_control(ASIC),
              use_register_access(ASIC.get_register_access()),
              use_readout_control(ASIC.get_readout_control()),
              use_histogram(histogram), use_repository(repository) {
        }

        [[nodiscard]] bool is_active() const override { return 0 < the_readout_countdown; }

        [[nodiscard]] Status_code start_test(Instruction_major *instruction) override {
            optional_instruction = instruction;
            const auto success{0 == the_readout_countdown};
            if (success) {
                test_prolog(Readout_count);
            }
            return Status_code(success);
        }

        void background_process() override {
            if (the_readout_countdown > 1) {
                if (test_loop()) {
                    the_readout_countdown--;
                }
            } else if (the_readout_countdown == 1) {
                the_readout_countdown = 0;
                test_epilog();
            }
        }

        [[nodiscard]] uint32_t get_diag_code() const { return the_diag_code; }

        void print_diag() const override;

    private:
        void test_prolog(int32_t count);

        void test_epilog();

        void update_settings();

        void test_result();

        [[nodiscard]] bool test_loop() const;

        static int32_t weighted_mean_index(const uint32_t *data, uint32_t size);
    };
} // IDE3380
