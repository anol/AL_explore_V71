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

        bool ISR_take(uint32_t &context_switch) override {
            return xSemaphoreTakeFromISR(
                optional_semaphore, reinterpret_cast<BaseType_t *>(&context_switch)) == pdTRUE;
        }

        bool ISR_give(uint32_t &context_switch) override {
            return xSemaphoreGiveFromISR(
                optional_semaphore, reinterpret_cast<BaseType_t *>(&context_switch)) == pdTRUE;
        }
    };
} // FreeRTOS
