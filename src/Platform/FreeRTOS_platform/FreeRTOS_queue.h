#pragma once
#include "Abstract_queue.h"

extern "C" {
#include "FreeRTOS.h"
#include "queue.h"
}

namespace FreeRTOS {
    template<typename T, uint32_t Queue_size>
    class FreeRTOS_queue : public Abstract::Abstract_queue<T> {
        StaticQueue_t the_queue_structure{};
        uint32_t      the_queue_storage[Queue_size]{};
        QueueHandle_t optional_queue{};

    public:
        FreeRTOS_queue() {
            optional_queue = xQueueCreateStatic(Queue_size, sizeof(void *), reinterpret_cast<uint8_t *>(the_queue_storage), &the_queue_structure);
            configASSERT(optional_queue != nullptr);
        }

        bool send(T item) override { return xQueueSend(optional_queue, item, eNoTasksWaitingTimeout) == pdPASS; }

        bool receive(T *item) override { return xQueueReceive(optional_queue, item, eNoTasksWaitingTimeout) == pdPASS; }

        bool ISR_send(T item) override {
            BaseType_t xHigherPriorityTaskWoken;
            return xQueueSendFromISR(optional_queue, item, &xHigherPriorityTaskWoken) == pdPASS;
        }

        bool ISR_receive(T *item) override {
            BaseType_t xHigherPriorityTaskWoken;
            return xQueueReceiveFromISR(optional_queue, item, &xHigherPriorityTaskWoken) == pdPASS;
        }
    };
} // FreeRTOS
