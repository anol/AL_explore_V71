//
// Created by aeols on 12.08.2026.
//

module;
#include "Persistent_parameter_id.h"
#include <cstdint>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include <cstdio>

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

using namespace IDE3380;

namespace Application
{
    void Hello_world::initialize()
    {
        constexpr UBaseType_t priority = tskIDLE_PRIORITY + 1;
        constexpr StackType_t stack_size = 1024 * 2;
        xTaskCreate(task_entry, "HelloWorld", stack_size, this, priority, &the_task);

        the_cadence_control.initialize();
        // // HAL_TIM_Base_Start_IT(&htim16); // USB Poll Timer
        // // HAL_Delay(500);
        the_bias.initialize();
        the_event_counter.initialize();
        the_console_service.initialize();
        the_command_handler.initialize();
        // the_histogram.disable_trigger_flag(true);
        // the_cadence_control.set_cadence_callback(nullptr, nullptr);
    }

    void Hello_world::run()
    {
        vTaskStartScheduler();
    }

    void Hello_world::task_entry(void* object)
    {
        static_cast<Hello_world*>(object)->task_loop();
    }

    void Hello_world::toggle_LED()
    {
        the_toggle_flag = !the_toggle_flag;
        if (the_toggle_flag)
        {
            use_board.get_pin(Domain::Pin_LED0).set();
            use_board.get_pin(Domain::Pin_LED1).clear();
        }
        else
        {
            use_board.get_pin(Domain::Pin_LED0).clear();
            use_board.get_pin(Domain::Pin_LED1).set();
        }
    }

    void Hello_world::task_loop()
    {
        constexpr TickType_t period = pdMS_TO_TICKS(1000);
        TickType_t last_wake_time = xTaskGetTickCount();
        Housekeeping_provider::print_version();
        the_repository.initialize();
        auto success = the_repository.load();
        the_command_handler.update_mode();
        if (success.success())
        {
            the_IDE3380.update_registers(the_repository, IDE3380_0);
        }
        else
        {
            printf("<> Failed to load repository <>\r\n");
        }
        success = the_bias.update_setpoints(the_repository);
        if (success.failed())
        {
            printf("<> Failed to update bias setpoints <>\r\n");
        }
        while (true)
        {
            vTaskDelayUntil(&last_wake_time, period);
            toggle_LED();
        }
    }
}
