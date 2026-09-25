//
// Created by aeols on 12.08.2026.
//

module;

module Application.Hello_world;
import Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Event_counter;
import Component.Bias_calibration;
import Type.Abstract_application;
import Type.Abstract_board;
import Application.Cadence_control;
import Application.Default_configuration;
import Application.Request_router;
import Support.Configuration_repository;
import Support.Mockup_persistent_storage;
import Support.Console_service;
import Domain.IO_pins;
import Application.Housekeeping_provider;
import Platform.FreeRTOS_task;

using namespace IDE3380;

namespace Application
{
    void Hello_world::initialize()
    {
         the_cadence_control.initialize();
        // // HAL_TIM_Base_Start_IT(&htim16); // USB Poll Timer
        // // HAL_Delay(500);
        the_event_counter.initialize();
        the_console_service.initialize();
        // the_histogram.disable_trigger_flag(true);
        // the_cadence_control.set_cadence_callback(nullptr, nullptr);
    }

    void Hello_world::run()
    {
        FreeRTOS::FreeRTOS_task::run();
    }

}
