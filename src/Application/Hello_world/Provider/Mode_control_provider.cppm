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
import Component.Bias_regulator;
import Platform.FreeRTOS_semaphore;
import Domain.Keyword_lookup;
import Application.SpectraNode_command_lookup;
import Application.SpectraNode_provider_indication;
import Application.Cadence_control;
import Application.Spectroscopic_data_provider;
import Application.Startup_task;
import Support.Configuration_repository;
import Support.Instruction_major;

using namespace IDE3380;

using namespace SpectraNode_interface;



using namespace Application;

export class Mode_control_provider : public Abstract_Mode_control_provider {
private:
    Histogram_storage &use_histogram;
    IDE3380::IDE3380_interface &use_IDE3380;
    Bias_regulator::Bias_regulator_task &use_bias_regulator;
    Repository::Configuration_repository &use_repository;
    Cadence_control the_cadence_control{};
    // The only provider-to-provider link: a mode change sets the channel and format of the data provider.
    Spectroscopic_data_provider &use_data_provider;
    Operation_mode the_mode{Mode_startup};
    Operation_mode the_operational_mode{Mode_idle}; ///< idle, nominal or demo; resumed after startup and calibration
    FreeRTOS::FreeRTOS_semaphore the_start_semaphore{}; ///< held by this controller in startup and calibration mode
    Startup_task the_startup_task;

public:
    Mode_control_provider(Histogram_storage &histogram,
                          IDE3380::IDE3380_interface &ASIC, Bias_regulator::Bias_regulator_task &bias_regulator,
                          Repository::Configuration_repository &repository,
                          Spectroscopic_data_provider &data_provider)
        : use_histogram(histogram),
          use_IDE3380(ASIC), use_bias_regulator(bias_regulator),
          use_repository(repository),
          use_data_provider(data_provider),
          the_startup_task(ASIC, bias_regulator.get_calibration(), repository, this,
                           [](void *user) { static_cast<Mode_control_provider *>(user)->end_startup(); }) {
        // The semaphore starts out empty, i.e. held here: the bias regulation waits until end_startup().
        use_bias_regulator.attach_start_semaphore(the_start_semaphore);
    }

    void initialize() {
        the_startup_task.initialize();
        the_cadence_control.initialize();
    }

    void print_status() const;

    Cadence_control &get_cadence_control() { return the_cadence_control; }

    [[nodiscard]] const Cadence_control &get_cadence_control() const { return the_cadence_control; }

    [[nodiscard]] bool is_demo_mode() const {
        return the_mode == Mode_demo;
    }

    [[nodiscard]] bool is_nominal_mode() const {
        return the_mode == Mode_nominal;
    }

    [[nodiscard]] bool is_calibration_mode() const {
        return the_mode == Mode_calibration;
    }

    /// Sets the operational mode (idle, nominal or demo). During startup or calibration it takes effect
    /// once that transient mode ends.
    void set_operation_mode(const Operation_mode mode) {
        the_operational_mode = mode;
        if (!is_transient_mode()) {
            the_mode = mode;
        }
    };

    /// Enter calibration mode, halting the bias regulation (after a pass in progress has finished), or
    /// leave it again for the operational mode. Fails during startup.
    [[nodiscard]] Status_code toggle_calibration_mode();

    Status_code set_mode(Operation_mode mode, Parameter_id channel, Parameter_id format, Parameter_id cadence);

    [[nodiscard]] Status_code update_mode();

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
    [[nodiscard]] bool is_transient_mode() const {
        return the_mode == Mode_startup || the_mode == Mode_calibration;
    }

    /// Called by the startup task once the configuration is loaded: leave startup mode.
    void end_startup();

    [[nodiscard]] Status_code set_parameter(Parameter_id param, int32_t value, bool success) const {
        return Status_code(use_repository.set(param, value).success() ? success : false);
    }

    [[nodiscard]] Status_code get_parameter(Parameter_id param, int32_t &value, bool success) const {
        return Status_code(use_repository.get(param, value).success() ? success : false);
    }

    static const char *Operation_mode_to_text(Operation_mode mode);
};
