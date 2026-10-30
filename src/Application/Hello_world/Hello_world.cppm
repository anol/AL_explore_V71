//
// Created by aeols on 12.08.2026.
//

module;
#include "Persistent_parameter_id.h"
#include <cstdint>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

export module Application.Hello_world;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Event_counter;
import Component.Bias_calibration;
import Type.Abstract_application;
import Type.Abstract_board;
import Application.Cadence_control;
import Application.Default_configuration;
import Application.Request_router;
import Domain.SpectraNode_command_table;
import Support.Configuration_repository;
import Support.Mockup_persistent_storage;
import Support.Console_service;
import Support.Heartbeat_task;
import Domain.IO_pins;
import Application.Primary_task;
import Application.Housekeeping_provider;
import Application.Mode_control_provider;
import Application.Spectroscopic_data_provider;

using namespace IDE3380;

export namespace Application
{
    class Hello_world : public Abstract::Abstract_application
    {
        const Default_configuration the_default_configuration;
        const SpectraNode_interface::SpectraNode_command_table the_command_table{};
        bool the_toggle_flag{};

        Abstract::Abstract_UART& use_UART;
        Abstract::Abstract_IO_pin& use_LED0;
        Abstract::Abstract_IO_pin& use_LED1;

        Cadence_control the_cadence_control{};
        IDE3380_interface the_IDE3380;
        Calibration::Bias_calibration the_bias;
        MOCKUP::Mockup_persistent_storage the_storage{};
        Repository::Configuration_repository the_repository{the_default_configuration, the_storage};
        Event_counter the_event_counter{};
        Histogram_storage the_histogram{the_event_counter, the_IDE3380};
        Mode_control_provider the_mode_control{
            the_histogram, the_IDE3380, the_bias, the_repository, the_cadence_control, the_data_provider
        };
        Spectroscopic_data_provider the_data_provider{
            the_histogram, the_repository, the_cadence_control, the_IDE3380
        };
        Request_router the_command_handler{
            use_UART, the_histogram, the_event_counter, the_IDE3380, the_bias, the_repository, the_cadence_control,
            the_mode_control, the_data_provider
        };
        Primary_task the_primary_task{the_IDE3380, the_bias, the_repository, the_command_handler};
        Support::Console_service the_console_service{use_UART, the_command_handler, the_command_table};
        Support::Heartbeat_task the_heartbeat_task{use_LED0, use_LED1};
        uint32_t the_background_count{};

    public:
        explicit Hello_world(Abstract::Abstract_board& board)
            : use_UART(board.get_UART()), use_LED0(board.get_pin(Domain::Pin_LED0)), use_LED1(board.get_pin(Domain::Pin_LED0)),
              the_IDE3380(board), the_bias(board)
        {
        }

        void initialize() override;

        void run() override;
    };
} // Application
