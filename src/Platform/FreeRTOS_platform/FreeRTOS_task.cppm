module;
#include <cstdint>
#include <cstdio>

#include "FreeRTOS.h"
#include "task.h"

export module Platform.FreeRTOS_task;
import Type.Abstract_task;


export namespace FreeRTOS {
    class FreeRTOS_task : public Abstract::Abstract_task {
        const char *the_name;
        TaskHandle_t optional_task{};
        TickType_t last_wake_time{};

    public:
        explicit FreeRTOS_task(const char *name, UBaseType_t priority, StackType_t stack_size)
            : the_name(name) {
            xTaskCreate(task_entry, name, stack_size, this, priority, &optional_task);
            configASSERT(optional_task);
        }

        void delay_until(const uint32_t millis) override { vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(millis)); }

        virtual void task_loop() = 0;

        static void run() {
            vTaskStartScheduler();
        }

    private:
        void prolog() {
            last_wake_time = xTaskGetTickCount();
            printf("Start %s\r\n", the_name);
        }

        static void task_entry(void *object) {
            if (object) {
                auto *task = static_cast<FreeRTOS_task *>(object);
                task->prolog();
                task->task_loop();
            }
        }
    };
}
