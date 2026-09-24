module;
#include "FreeRTOS.h"
#include "semphr.h"

export module Platform.FreeRTOS_semaphore;
import Type.Abstract_semaphore;


export namespace FreeRTOS {
    class FreeRTOS_semaphore : public Abstract::Abstract_semaphore {
        StaticSemaphore_t the_semaphore_structure{};
        SemaphoreHandle_t optional_semaphore{};

    public:
        FreeRTOS_semaphore() {
            optional_semaphore = xSemaphoreCreateBinaryStatic(&the_semaphore_structure);
            configASSERT(optional_semaphore != nullptr);
        }

        bool take() override {
            return xSemaphoreTake(optional_semaphore, portMAX_DELAY) == pdTRUE;
        }

        bool give() override {
            return xSemaphoreGive(optional_semaphore) == pdTRUE;
        }

        bool ISR_take(bool &context_switch) override {
            BaseType_t xHigherPriorityTaskWoken;
            auto success = xSemaphoreTakeFromISR(optional_semaphore, &xHigherPriorityTaskWoken) == pdTRUE;
            context_switch = xHigherPriorityTaskWoken == pdTRUE;
            return success;
        }

        bool ISR_give(bool &context_switch) override {
            BaseType_t xHigherPriorityTaskWoken;
            auto success = xSemaphoreGiveFromISR(optional_semaphore, &xHigherPriorityTaskWoken) == pdTRUE;
            context_switch = xHigherPriorityTaskWoken == pdTRUE;
            return success;
        }
    };
} // FreeRTOS
