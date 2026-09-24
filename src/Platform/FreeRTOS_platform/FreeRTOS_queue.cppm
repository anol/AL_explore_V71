module;

#include <cstdint>
#include "FreeRTOS.h"
#include "queue.h"

export module Platform.FreeRTOS_queue;
import Type.Abstract_queue;


export namespace FreeRTOS {
    template<typename T, uint32_t Queue_size>
    class FreeRTOS_queue : public Abstract::Abstract_queue<T> {
        StaticQueue_t the_queue_structure{};
        T the_queue_storage[Queue_size]{};
        QueueHandle_t optional_queue{};

    public:
        FreeRTOS_queue() {
            optional_queue = xQueueCreateStatic(Queue_size, sizeof(T), reinterpret_cast<uint8_t *>(the_queue_storage), &the_queue_structure);
            configASSERT(optional_queue != nullptr);
        }

        bool send(T item) override {
            return xQueueSend(optional_queue, &item, eNoTasksWaitingTimeout) == pdPASS;
        }

        bool receive(T *item) override {
            return xQueueReceive(optional_queue, item, eNoTasksWaitingTimeout) == pdPASS;
        }

        bool ISR_send(T item, uint32_t &context_switch) override {
            return xQueueSendFromISR(
                optional_queue, &item, reinterpret_cast<BaseType_t *>(&context_switch)) == pdPASS;
        }

        bool ISR_receive(T *item, uint32_t &context_switch) override {
            return xQueueReceiveFromISR(
                       optional_queue, item, reinterpret_cast<BaseType_t *>(&context_switch)) == pdPASS;
        }
    };
} // FreeRTOS
