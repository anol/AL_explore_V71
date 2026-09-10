//
// Created by aeols on 12.08.2026.
//

#include <cstdio>
#include "Hello_world.h"

#include "IO_pins.h"

namespace Application
{
    void Hello_world::initialize()
    {
        constexpr UBaseType_t priority = tskIDLE_PRIORITY + 1;
        constexpr StackType_t stack_size =1024 * 2;
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
            use_board.get_pin(Dictionary::Pin_LED0).set();
            use_board.get_pin(Dictionary::Pin_LED1).clear();
        }
        else
        {
            use_board.get_pin(Dictionary::Pin_LED0).clear();
            use_board.get_pin(Dictionary::Pin_LED1).set();
        }
    }

    void Hello_world::task_loop()
    {
        constexpr TickType_t period = pdMS_TO_TICKS(1000);
        TickType_t last_wake_time = xTaskGetTickCount();
        printf("Hello_world::task_loop\r\n");
        while (true)
        {
            vTaskDelayUntil(&last_wake_time, period);
            toggle_LED();
        }
    }
}
