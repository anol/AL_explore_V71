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


// #include "Component/MCU/STM32U575RG/U575xG_embedded_flash.h"
// #include "Component/MCU/STM32U575RG/U575xG_persistent_storage.h"

export namespace Application
{
    class Hello_world : public Abstract::Abstract_application
    {
        Abstract::Abstract_board& use_board;
        TaskHandle_t the_task{};
        bool the_toggle_flag{};

        const Default_configuration the_default_configuration;
        Cadence_control the_cadence_control{};
        IDE3380_interface the_IDE3380;
        Calibration::Bias_calibration the_bias{};
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
        Housekeeping_provider the_housekeeper;
        Request_router the_command_handler{
            the_histogram, the_event_counter, the_IDE3380, the_bias, the_repository, the_cadence_control,
            the_housekeeper, the_mode_control, the_data_provider
        };
        SpectraNode_interface::SpectraNode_command_table the_command_table{};
        Primary_task the_primary_task{the_IDE3380, the_bias, the_repository, the_command_handler};
        Support::Console_service the_console_service{use_board.get_UART(), the_command_handler, the_command_table};
        Support::Heartbeat_task the_heartbeat_task{
            use_board.get_pin(Domain::Pin_LED0), use_board.get_pin(Domain::Pin_LED1)
        };
        uint32_t the_background_count{};

    public:
        explicit Hello_world(Abstract::Abstract_board& board)
            : use_board(board), the_IDE3380(board),
              the_housekeeper(board,
                              the_histogram, the_IDE3380, the_bias, the_repository,
                              the_cadence_control, the_data_provider, the_mode_control
              )
        {
        }

        void initialize() override;

        void run() override;
    };
} // Application
