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
* @file   Science_data_provider.cppm
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief
*/

module;
#include <cstdint>
#include "Persistent_parameter_id.h"

export module Application.Science_data_provider;
import Component.IDE3380_provider;
import Component.Event_counter;
import Component.Histogram_storage;
import Domain.Keyword_lookup;
import Application.AstraLEO_command_lookup;
import Application.AstraLEO_provider_indication;
import Application.Cadence_control;
import Support.Configuration_repository;
import Support.Instruction_major;

using namespace IDE3380;

using namespace AstraLEO_interface;



export class Science_data_provider : public Abstract_Science_data_provider {
    Repository::Configuration_repository &use_repository;
    Application::Cadence_control &use_cadence;
    IDE3380::IDE3380_provider &use_IDE3380;
    Application::Event_counter the_event_counter{};
    Application::Histogram_storage the_histogram{the_event_counter, use_IDE3380.get_readout_control()};
    Application::Data_format the_format{};
    int the_channel{};

public:
    Science_data_provider(Repository::Configuration_repository &repository,
                          Application::Cadence_control &cadence, IDE3380::IDE3380_provider &ASIC)
        : use_repository(repository), use_cadence(cadence),
          use_IDE3380(ASIC) {
    }

    void initialize() {
        the_event_counter.initialize();
    }

    Application::Histogram_storage &get_histogram() { return the_histogram; }

    Application::Event_counter &get_event_counter() { return the_event_counter; }

    void set_format(const Application::Data_format format) {
        the_format = format;
    }

    void set_channel(const int channel) {
        the_channel = channel;
        use_IDE3380.set_ignore_trigger(channel == 0);
    }

    void send_science_data(uint32_t cadence);

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
