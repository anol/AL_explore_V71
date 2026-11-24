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
import Component.IDE3380_provider;
import Component.Bias_calibration;
import Component.Bias_regulator;
import Platform.FreeRTOS_semaphore;
import Domain.Keyword_lookup;
import Application.AstraLEO_command_lookup;
import Application.AstraLEO_provider_indication;
import Application.Cadence_control;
import Application.Science_data_provider;
import Application.Startup_task;
import Support.Configuration_repository;
import Support.Instruction_major;
import Component.Temperature_sensor;
import Application.Housekeeping_provider;

using namespace IDE3380;

using namespace AstraLEO_interface;


using namespace Application;

export class Mode_control_provider : public Abstract_Mode_control_provider {
private:
    IDE3380::IDE3380_provider &use_IDE3380;
    Repository::Configuration_repository &use_repository;
    Cadence_control the_cadence_control{};
    // The only provider-to-provider link: a mode change sets the channel and format of the data provider.
    Science_data_provider &use_data_provider;
    Operation_mode the_mode{Mode_startup};
    Operation_mode the_operational_mode{Mode_idle};     ///< idle, nominal or demo; resumed after startup and calibration
    FreeRTOS::FreeRTOS_semaphore the_start_semaphore{}; ///< held by this controller in startup and calibration mode
    Startup_task the_startup_task;

public:
    Mode_control_provider(IDE3380::IDE3380_provider &ASIC,
                          Repository::Configuration_repository &repository,
                          Science_data_provider &data_provider)
        : use_IDE3380(ASIC),
          use_repository(repository),
          use_data_provider(data_provider),
          the_startup_task(ASIC, repository, this,
                           [](void *user) { static_cast<Mode_control_provider *>(user)->end_startup(); }) {
    }

    void attach_housekeeper(Housekeeping_provider &housekeeper) {
        auto &bias_regulator = housekeeper.get_bias_regulator();
        auto &temperature_sensor = housekeeper.get_temperature_sensor();
        bias_regulator.attach_start_semaphore(the_start_semaphore);
        temperature_sensor.attach_start_semaphore(the_start_semaphore);
        the_startup_task.attach_bias(bias_regulator.get_calibration());
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

    [[nodiscard]] const char *get_mode_text() const {
        return Operation_mode_to_text(the_mode);
    }

    void set_operation_mode(Operation_mode mode);

    [[nodiscard]] Status_code enter_calibration_mode();

    Status_code set_mode(Operation_mode mode, Parameter_id channel, Parameter_id format, Parameter_id cadence);

    [[nodiscard]] Status_code update_mode();

protected:
    void v_MODE_CAL(Instruction_major &) override;

    void v_MODE_DEMO(Instruction_major &) override;

    void v_MODE_IDLE(Instruction_major &) override;

    void v_MODE_NOMINAL(Instruction_major &) override;

    void v_DEMO_CADENCE_seconds(Instruction_major &, int seconds_2) override;

    void v_DEMO_CHANNEL_channel(Instruction_major &, int channel_2) override;

    void v_NOMINAL_ALARM_micro_sievert(Instruction_major &, int micro_sievert_2) override;

    void v_NOMINAL_CADENCE_seconds(Instruction_major &, int seconds_2) override;

private:
    void end_startup();

    [[nodiscard]] Status_code set_parameter(Parameter_id param, int32_t value, bool success) const {
        return Status_code(use_repository.set(param, value).success() ? success : false);
    }

    [[nodiscard]] Status_code get_parameter(Parameter_id param, int32_t &value, bool success) const {
        return Status_code(use_repository.get(param, value).success() ? success : false);
    }

    static const char *Operation_mode_to_text(Operation_mode mode);
};
