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


#pragma once

#include "CLI_help.h"
#include "Generated_code/SpectraNode_provider_indication.h"

class Mode_control_provider;
class Spectroscopic_data_provider;

namespace Repository
{
    class Configuration_repository;
}

namespace Calibration
{
    class Bias_calibration;
}

namespace IDE3380
{
    class IDE3380_interface;
}

namespace Application
{
    class Cadence_control;
    class Histogram_storage;
}

class Housekeeping_provider : public Abstract_Housekeeping_provider
{
    CLI_help the_help{Key_HELP, get_commands(), get_keyword};
    Application::Histogram_storage& use_histogram;
    IDE3380::IDE3380_interface& use_IDE3380;
    Calibration::Bias_calibration& use_bias;
    Repository::Configuration_repository& use_repository;
    Application::Cadence_control& use_cadence;
    Spectroscopic_data_provider& use_data_provider;
    Mode_control_provider& use_mode_control;

public:
    Housekeeping_provider(Application::Histogram_storage& histogram,
                          IDE3380::IDE3380_interface& ASIC, Calibration::Bias_calibration& bias,
                          Repository::Configuration_repository& repository,
                          Application::Cadence_control& cadence,
                          Spectroscopic_data_provider& data_provider,
                          Mode_control_provider& mode_control)
        : use_histogram(histogram), use_IDE3380(ASIC),
          use_bias(bias),
          use_repository(repository),
          use_cadence(cadence),
          use_data_provider(data_provider),
          use_mode_control(mode_control)
    {
    }

    static void print_version();

protected:
    void v_VERSION(Instruction_major&) override;

    void v_STATUS(Instruction_major&) override;

    void v_DIAG(Instruction_major&) override;

    void v_TEST(Instruction_major&) override;

    void v_HELP(Instruction_major&) override;
};
