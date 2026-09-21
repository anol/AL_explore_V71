//
// Created by aeols on 12.08.2026.
//

#pragma once
import Type.Abstract_application;
import Type.Abstract_board;

#include "Cadence_control.h"
#include "Request_router.h"
#include "Default_configuration.h"
// #include "Component/MCU/STM32U575RG/U575xG_embedded_flash.h"
// #include "Component/MCU/STM32U575RG/U575xG_persistent_storage.h"
import Support.Configuration_repository;
#include "IDE3380_interface.h"
#include "../../Component/IDE3380/Histogram_storage.h"
#include "../../Component/IDE3380/Event_counter.h"
#include "Bias_calibration.h"
import Support.Mockup_persistent_storage;

extern "C" {
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
}

import Support.Console_service;

namespace Application
{
    class Hello_world : public Abstract::Abstract_application
    {
        Abstract::Abstract_board& use_board;
        TaskHandle_t the_task{};
        bool the_toggle_flag{};

        const Default_configuration the_attribute_types;
        Cadence_control the_cadence_control{};
        IDE3380_interface the_IDE3380;
        Calibration::Bias_calibration the_bias{};
        MOCKUP::Mockup_persistent_storage the_storage{};
        Configuration_repository the_repository{the_attribute_types, the_storage};
        Event_counter the_event_counter{};
        Histogram_storage the_histogram{the_event_counter, the_IDE3380};
        Request_router the_command_handler{
            the_histogram, the_event_counter, the_IDE3380, the_bias, the_repository, the_cadence_control
        };
        Console::Console_service the_console_service{use_board.get_UART(), the_command_handler};
        uint32_t the_background_count{};

    public:
        explicit Hello_world(Abstract::Abstract_board& board) : use_board(board), the_IDE3380(board)
        {
        }

        void initialize() override;

        void run() override;

    private:
        void task_loop();

        static void task_entry(void* object);

        void toggle_LED();
    };
} // Application
