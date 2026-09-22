module;
#include "FreeRTOS.h"
#include "semphr.h"

export module Platform.FreeRTOS_semaphore;
import Type.Abstract_semaphore;


export namespace FreeRTOS
{
    class FreeRTOS_semaphore : public Abstract::Abstract_semaphore
    {
        StaticSemaphore_t the_semaphore_structure{};
        SemaphoreHandle_t optional_semaphore{};

    public:
        FreeRTOS_semaphore() { optional_semaphore = xSemaphoreCreateBinaryStatic(&the_semaphore_structure); }

        bool take() override { return xSemaphoreTake(optional_semaphore, portMAX_DELAY) == pdTRUE; }

        bool give() override
        {
            BaseType_t xHigherPriorityTaskWoken;
            return xSemaphoreGiveFromISR(optional_semaphore, &xHigherPriorityTaskWoken) == pdTRUE;
        }
    };
} // FreeRTOS
