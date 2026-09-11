//
// Created by aeols on 2026-09-10.
//

module;
#include <Abstract_task.h>

module Support.Console_service;

namespace Console
{
    void Console_transmit_task::initialize()
    {
        constexpr UBaseType_t priority = tskIDLE_PRIORITY + 1;
        constexpr StackType_t stack_size = configMINIMAL_STACK_SIZE * 2;
        xTaskCreate(task_entry, "Console_transmit_task", stack_size, this, priority, &optional_task);
    }

    void Console_transmit_task::task_entry(void* object)
    {
        static_cast<Console_transmit_task*>(object)->task_loop();
    }

    void Console_transmit_task::task_loop()
    {
        constexpr TickType_t period = pdMS_TO_TICKS(1000);
        TickType_t last_wake_time = xTaskGetTickCount();
        while (true)
        {
            vTaskDelayUntil(&last_wake_time, period);
        }
    }
} // Console
