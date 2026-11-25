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
* @file   Housekeeping_provider.h
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief
*/

module;
#include "Persistent_parameter_id.h"
#include <cstdint>

export module Application.Housekeeping_provider;
import Component.Histogram_storage;
import Component.IDE3380_provider;
import Component.Bias_calibration;
import Component.Bias_regulator;
import Component.Temperature_sensor;
import Domain.Keyword_lookup;
import Application.AstraLEO_command_lookup;
import Application.AstraLEO_provider_indication;
import Application.Cadence_control;
import Support.Configuration_repository;
import Support.Instruction_major;
import Type.Abstract_board;
import Support.Console_support;

using namespace IDE3380;

using namespace Domain_interface;
using namespace AstraLEO_interface;


export class Housekeeping_provider : public Abstract_Housekeeping_provider {
public:
    using Status_func = void (*)(void *user);

private:
    Abstract::Abstract_board &use_board;
    Support::Console_support &use_console;
    Application::Histogram_storage &use_histogram;
    IDE3380_provider &use_IDE3380;
    Repository::Configuration_repository &use_repository;
    Application::Cadence_control &use_cadence;
    Temperature_sensor::Temperature_task the_temperature_task;
    Bias_regulator::Bias_regulator_task the_bias_regulator_task;
    void *use_status_user;
    Status_func use_status_func;
    static bool the_trace_flag;

public:
    Housekeeping_provider(Abstract::Abstract_board &board,
                          Support::Console_support &console,
                          Application::Histogram_storage &histogram,
                          IDE3380::IDE3380_provider &ASIC,
                          Repository::Configuration_repository &repository,
                          Application::Cadence_control &cadence,
                          void *status_user, const Status_func status_func)
        : use_board(board),
          use_console(console),
          use_histogram(histogram),
          use_IDE3380(ASIC),
          use_repository(repository),
          use_cadence(cadence),
          the_temperature_task(board),
          the_bias_regulator_task(board, the_temperature_task),
          use_status_user(status_user),
          use_status_func(status_func) {}

    void initialize() {
        the_bias_regulator_task.initialize();
        the_temperature_task.initialize();
    }

    Bias_regulator::Bias_regulator_task &get_bias_regulator() { return the_bias_regulator_task; }

    Temperature_sensor::Temperature_task &get_temperature_sensor() { return the_temperature_task; }

    static bool is_trace() { return the_trace_flag; }

    static void print_version();

private:
    void print_housekeeping() const;

    void print_ADC_channels() const;

protected:
    void v_DIAG(Instruction_major &) override;

    void v_ECHO(Instruction_major &) override;

    void v_HELP(Instruction_major &) override;

    void v_HOUSE(Instruction_major &) override;

    void v_STATUS(Instruction_major &) override;

    void v_TEST(Instruction_major &) override;

    void v_TRACE(Instruction_major &) override;

    void v_VERSION(Instruction_major &) override;
};
