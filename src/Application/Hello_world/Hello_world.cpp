//
// Created by aeols on 12.08.2026.
//

module;

module Application.Hello_world;
import Component.Event_counter;
import Application.Cadence_control;
import Support.Console_service;
import Platform.FreeRTOS_task;

using namespace IDE3380;

namespace Application
{
    void Hello_world::initialize()
    {
        the_cadence_control.initialize();
        the_event_counter.initialize();
        the_console_service.initialize();
        // the_histogram.disable_trigger_flag(true);
        // the_cadence_control.set_cadence_callback(nullptr, nullptr);

    }

    void Hello_world::run()
    {
        FreeRTOS::FreeRTOS_task::start_scheduler();
    }
}
