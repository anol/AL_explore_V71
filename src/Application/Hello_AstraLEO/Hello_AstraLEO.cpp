//
// Created by aeols on 12.08.2026.
//

module;

module Application.Hello_AstraLEO;
import Platform.FreeRTOS_task;

namespace Application
{
    void Hello_AstraLEO::initialize()
    {
        the_service_provider.initialize();
        the_console.initialize();
        // the_histogram.disable_trigger_flag(true);
        // the_cadence_control.set_cadence_callback(nullptr, nullptr);

    }

    void Hello_AstraLEO::run()
    {
        FreeRTOS::FreeRTOS_task::start_scheduler();
    }
}
