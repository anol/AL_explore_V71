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
* @file   Mode_control_provider.h
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief
*/

module;
#include <cstdint>
#include "Persistent_parameter_id.h"

export module Application.Mode_control_provider;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Bias_calibration;
import Domain.SpectraNode_keyword_lookup;
import Domain.SpectraNode_command_lookup;
import Domain.SpectraNode_provider_indication;
import Application.Cadence_control;
import Application.Spectroscopic_data_provider;
import Support.Configuration_repository;
import Support.Instruction_major;

using namespace IDE3380;

using namespace SpectraNode_interface;



using namespace Application;

export class Mode_control_provider : public Abstract_Mode_control_provider {
private:
    Histogram_storage &use_histogram;
    IDE3380::IDE3380_interface &use_IDE3380;
    Calibration::Bias_calibration &use_bias;
    Repository::Configuration_repository &use_repository;
    Cadence_control &use_cadence;
    Spectroscopic_data_provider &use_data_provider;
    Operation_mode the_mode{};

public:
    Mode_control_provider(Histogram_storage &histogram,
                          IDE3380::IDE3380_interface &ASIC, Calibration::Bias_calibration &bias,
                          Repository::Configuration_repository &repository,
                          Cadence_control &cadence,
                          Spectroscopic_data_provider &data_provider)
        : use_histogram(histogram),
          use_IDE3380(ASIC), use_bias(bias),
          use_repository(repository), use_cadence(cadence),
          use_data_provider(data_provider) {
    }

    void print_status() const;

    [[nodiscard]] bool is_demo_mode() const {
        return the_mode == Mode_demo;
    }

    [[nodiscard]] bool is_nominal_mode() const {
        return the_mode == Mode_nominal;
    }

    void set_operation_mode(const Operation_mode mode) {
        the_mode = mode;
    };

    Status_code set_mode(Operation_mode mode, Parameter_id channel, Parameter_id format, Parameter_id cadence);

protected:
    void v_MODE_DEMO(Instruction_major &) override;

    void v_MODE_IDLE(Instruction_major &) override;

    void v_MODE_NOMINAL(Instruction_major &) override;

    void v_IDLE_CADENCE_seconds(Instruction_major &, int seconds_2) override;

    void v_DEMO_CADENCE_seconds(Instruction_major &, int seconds_2) override;

    void v_DEMO_CHANNEL_channel(Instruction_major &, int channel_2) override;

    void v_NOMINAL_ALARM_micro_sievert(Instruction_major &, int micro_sievert_2) override;

    void v_NOMINAL_CADENCE_seconds(Instruction_major &, int seconds_2) override;

private:
    [[nodiscard]] Status_code set_parameter(Parameter_id param, int32_t value, bool success) const {
        return Status_code(use_repository.set(param, value).success() ? success : false);
    }

    [[nodiscard]] Status_code get_parameter(Parameter_id param, int32_t &value, bool success) const {
        return Status_code(use_repository.get(param, value).success() ? success : false);
    }

    static const char *Operation_mode_to_text(Operation_mode mode);
};
