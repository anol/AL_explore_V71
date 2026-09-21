

module;
#include <cstdint>
#include "FreeRTOS.h"
#include "task.h"

export module Platform.FreeRTOS_task;
import Type.Abstract_task;



export namespace FreeRTOS {
    class FreeRTOS_task : public Abstract::Abstract_task {
    public:
        enum { Default_priority = tskIDLE_PRIORITY + 1, Default_stack_size = configMINIMAL_STACK_SIZE * 2 };

    private:
        TaskHandle_t optional_task{};
        TickType_t   last_wake_time{};

    public:
        FreeRTOS_task(const char *name, UBaseType_t priority = Default_priority, StackType_t stack_size = Default_stack_size) {
            xTaskCreate(task_entry, name, stack_size, this, priority, &optional_task);
        }

        void delay_until(const uint32_t millis) override { vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(millis)); }

        virtual void task_loop() = 0;

    private:
        void prolog() {
            last_wake_time = xTaskGetTickCount();
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
