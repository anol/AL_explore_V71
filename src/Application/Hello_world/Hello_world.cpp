//
// Created by aeols on 12.08.2026.
//

#include <stdio.h>
#include "Hello_world.h"

#include "IO_pins.h"

namespace Application
{
    void Hello_world::initialize()
    {
        constexpr UBaseType_t priority = tskIDLE_PRIORITY + 1;
        constexpr StackType_t stack_size = configMINIMAL_STACK_SIZE * 2;
        xTaskCreate(task_entry, "HelloWorld", stack_size, this, priority, &the_task);

        // the_cadence_control.initialize();
        // // HAL_TIM_Base_Start_IT(&htim16); // USB Poll Timer
        // // HAL_Delay(500);
        // the_bias.initialize();
        // the_event_counter.initialize();
        // the_command_parser.initialize();
        // the_command_handler.initialize();
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
        TickType_t last_wake_time = xTaskGetTickCount();
        printf("Hello_world::task_loop\r\n");
        use_board.print_diagnostics();
        auto& console = use_board.get_UART();
        while (true)
        {
            if (console.has_input())
            {
                console.for_each_input(this, [](void* user, const uint32_t data)
                {
                    printf("%c", static_cast<char>(data));
                    return true;
                });
                constexpr TickType_t period = pdMS_TO_TICKS(500);
                vTaskDelayUntil(&last_wake_time, period);
            }
            else
            {
                constexpr TickType_t period = pdMS_TO_TICKS(1000);
                vTaskDelayUntil(&last_wake_time, period);
                printf(".");
                toggle_LED();
            }
        }
    }
} // Application
