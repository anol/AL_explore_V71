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
import Component.IDE3380_provider;
import Component.Bias_calibration;
import Component.Event_counter;
import Domain.Keyword_lookup;
import Application.SpectraNode_command_lookup;
import Application.SpectraNode_provider_indication;
import Support.Configuration_repository;
import Support.Instruction_major;
import Application.Mode_control_provider;

using namespace IDE3380;

using namespace SpectraNode_interface;


export class Instrument_calibration_provider : public Abstract_Instrument_calibration_provider {
    Calibration::Bias_calibration &use_bias;
    Repository::Configuration_repository &use_repository;
    Mode_control_provider &use_mode_control;
    Calibration::IDE3380_test_pedestal the_test_pedestal;
    Calibration::IDE3380_test_noise_floor the_test_noise_floor;
    bool the_trace_flag{};

public:
    Instrument_calibration_provider(Calibration::Bias_calibration &bias,
                                    Repository::Configuration_repository &repository,
                                    IDE3380_provider &IDE3380,
                                    Application::Histogram_storage &histogram,
                                    Application::Event_counter &event_counter,
                                    Mode_control_provider &mode_control)
        : use_bias(bias), use_repository(repository), use_mode_control(mode_control),
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

    void print_status() const {
        use_bias.print_status();
    }

protected:
    void v_BIAS(Instruction_major &) override;

    void v_BIAS_ADC(Instruction_major &) override;

    void v_BIAS_ADC_SET(Instruction_major &) override;

    void v_BIAS_ADC_SET_v30_v40(Instruction_major &, int v30_2, int v40_3) override;

    void v_BIAS_C25(Instruction_major &) override;

    void v_BIAS_C25_bias_mV(Instruction_major &, int bias_mV_2) override;

    void v_BIAS_DAC(Instruction_major &) override;

    void v_BIAS_DAC_data12(Instruction_major &, int data12_2) override;

    void v_BIAS_DAC_SET(Instruction_major &) override;

    void v_BIAS_DAC_SET_v30_v40(Instruction_major &, int v30_3, int v40_4) override;

    void v_BIAS_DIAG(Instruction_major &) override;

    void v_BIAS_ERROR(Instruction_major &) override;

    void v_BIAS_SET(Instruction_major &) override;

    void v_BIAS_SET_bias_mV(Instruction_major &, int bias_mV_2) override;

    void v_BIAS_TEMP(Instruction_major &) override;

    void v_BIAS_TEMP_SET(Instruction_major &) override;

    void v_BIAS_TEMP_SET_a_b(Instruction_major &, int a_3, int b_4) override;

    void v_BIAS_VOLT(Instruction_major &) override;

    void v_CAL_DUMP(Instruction_major &) override;

    void v_CAL_DIAG(Instruction_major &) override;

    void v_CAL_TEST_BIAS(Instruction_major &) override;

    void v_CAL_TEST_OFFSET(Instruction_major &) override;

    void v_CAL_TEST_NOISE(Instruction_major &) override;

    void v_CAL_TEST_PEDESTAL(Instruction_major &) override;

    void v_CAL_TRACE(Instruction_major &) override;

private:
    [[nodiscard]] bool require_calibration_mode(Instruction_major &instruction) const;

    void override_setpoint(Instruction_major &instruction, int bias_mV);
};
