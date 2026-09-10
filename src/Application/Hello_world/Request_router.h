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
* @file   Command_indication.h
* @author AndersEmilOlsen, IDEAS
* @date   14.04.2026
* @brief  
*/


#pragma once

#include "Abstract_task.h"
#include "Cadence_control.h"
#include "Configuration_manager_provider.h"
#include "Housekeeping_provider.h"
#include "Instrument_calibration_provider.h"
#include "Mode_control_provider.h"
#include "Spectroscopic_data_provider.h"

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
    class Event_counter;

    class Request_router
    {
        Histogram_storage& use_histogram;
        IDE3380_interface& use_IDE3380;
        Calibration::Bias_calibration& use_bias;
        Repository::Configuration_repository& use_repository;
        Cadence_control& use_cadence;
        Event_counter& use_event_counter;
        Mode_control_provider the_mode_control{
            use_histogram, use_IDE3380, use_bias, use_repository, use_cadence, the_data_provider
        };
        Spectroscopic_data_provider the_data_provider{
            use_histogram, use_repository, use_cadence, use_IDE3380
        };
        Instrument_calibration_provider the_instrument_calibration{
            use_bias, use_repository, use_IDE3380, use_histogram, use_event_counter
        };
        Configuration_manager_provider the_configuration_manager{use_repository, use_IDE3380};
        Housekeeping_provider the_housekeeper{
            use_histogram, use_IDE3380, use_bias, use_repository, use_cadence, the_data_provider, the_mode_control
        };
        int deepsleep_active{};
        int prevent_sleep{};
        int send_debug_data{};
        int deepsleep_enable{};

    public:
        explicit Request_router(Histogram_storage& histogram, Event_counter& counter,
                                IDE3380_interface& ASIC, Calibration::Bias_calibration& bias,
                                Repository::Configuration_repository& repository, Cadence_control& cadence)
            : use_histogram(histogram), use_IDE3380(ASIC), use_bias(bias),
              use_repository(repository), use_cadence(cadence), use_event_counter(counter)
        {
        }

        void initialize();

        void on_indication(Instruction_major& instruction);

        bool background_process()
        {
            return the_instrument_calibration.background_process();
        }

        void set_output_channel(const uint8_t channel)
        {
            the_data_provider.set_channel(channel);
        }

        void set_output_format(uint8_t data)
        {
            the_data_provider.set_format(static_cast<Data_format>(data));
        }

        void set_operation_mode(Operation_mode mode)
        {
            the_mode_control.set_operation_mode(mode);
        }

        void send_science_data() const
        {
            if (the_mode_control.is_nominal_mode() || the_mode_control.is_demo_mode())
            {
                const auto cadence = use_cadence.get_cadence();
                the_data_provider.send_science_data(cadence);
            }
        }

        void update_histogram(const TXD_data_t& data) const
        {
            use_histogram.update_histogram(data);
        }

        void wakeup_from_sleep()
        {
        }

        Status_code update_mode();
    };
} // Application
