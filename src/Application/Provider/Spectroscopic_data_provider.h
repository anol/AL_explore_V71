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
* @file   Spectroscopic_data_provider.h
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief
*/


#pragma once
#include "Persistent_parameter_id.h"
#include "Generated_code/SpectraNode_provider_indication.h"
#include "IDE3380_interface.h"
#include "IDE3380_readout_control.h"

namespace Repository {
    class Configuration_repository;
}

namespace Application {
    class Cadence_control;
    class Histogram_storage;
}

class Spectroscopic_data_provider : public Abstract_Spectroscopic_data_provider {
    Application::Histogram_storage &use_histogram;
    Repository::Configuration_repository &use_repository;
    Application::Cadence_control &use_cadence;
    IDE3380::IDE3380_readout_control &use_readout_control;
    Application::Data_format the_format{};
    int the_channel{};

public:
    Spectroscopic_data_provider(Application::Histogram_storage &histogram, Repository::Configuration_repository &repository,
                                Application::Cadence_control &cadence, IDE3380::IDE3380_interface &ASIC)
        : use_histogram(histogram), use_repository(repository), use_cadence(cadence),
          use_readout_control(ASIC.get_readout_control()) {
    }

    void set_format(const Application::Data_format format) {
        the_format = format;
    }

    void set_channel(const int channel) {
        the_channel = channel;
        use_readout_control.set_ignore_trigger(channel == 0);
    }

    void send_science_data(uint32_t cadence) const;

    void print_status() const;

protected:
    void v_GET(Instruction_major &) override;

    void v_GET_RESET(Instruction_major &) override;

    void v_FORMAT_R6(Instruction_major &) override;

    void v_FORMAT_CPS(Instruction_major &) override;

    void v_FORMAT_N42(Instruction_major &) override;

    void v_TIME_date_time(Instruction_major &, int date_1, int time_2) override;

private:
    static const char *format_to_text(Application::Data_format format);
};
