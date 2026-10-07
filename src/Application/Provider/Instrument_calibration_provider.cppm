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
* @file   Instrument_calibration_provider.h
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief
*/

module;
#include "Persistent_parameter_id.h"
#include <cstdint>

export module Application.Instrument_calibration_provider;
import Component.IDE3380_test_pedestal;
import Component.IDE3380_test_noise_floor;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Component.Event_counter;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;

using namespace IDE3380;

using namespace SpectraNode_interface;


export class Instrument_calibration_provider : public Abstract_Instrument_calibration_provider {
    Calibration::Bias_calibration &use_bias;
    Repository::Configuration_repository &use_repository;
    Calibration::IDE3380_test_pedestal the_test_pedestal;
    Calibration::IDE3380_test_noise_floor the_test_noise_floor;
    bool the_trace_flag{};

public:
    Instrument_calibration_provider(Calibration::Bias_calibration &bias,
                                    Repository::Configuration_repository &repository,
                                    IDE3380_interface &IDE3380,
                                    Application::Histogram_storage &histogram,
                                    Application::Event_counter &event_counter)
        : use_bias(bias), use_repository(repository),
          the_test_pedestal(IDE3380, histogram, repository),
          the_test_noise_floor(IDE3380, event_counter, repository) {
    }

    bool background_process() {
        bool busy{};
        if (the_test_pedestal.is_active()) {
            the_test_pedestal.background_process();
            busy = true;
        } else if (the_test_noise_floor.is_active()) {
            the_test_noise_floor.background_process();
            busy = true;
        }
        return busy;
    }

protected:
    void v_CAL_TRACE(Instruction_major &) override;

    void v_CAL_ADC_V35_cal_35V(Instruction_major &, int cal_35V_3) override;

    void v_CAL_ADC_V40_cal_40V(Instruction_major &, int cal_40V_3) override;

    void v_CAL_DAC_V35_cal_35V(Instruction_major &, int cal_35V_3) override;

    void v_CAL_DAC_V40_cal_40V(Instruction_major &, int cal_40V_3) override;

    void v_CAL_DAC_data16(Instruction_major &, int data16_2) override;

    void v_CAL_TEST_BIAS(Instruction_major &) override;

    void v_CAL_TEST_OFFSET(Instruction_major &) override;

    void v_CAL_TEST_NOISE(Instruction_major &) override;

    void v_CAL_TEST_PEDESTAL(Instruction_major &) override;

    void v_CAL_DIAG(Instruction_major &) override;
};
