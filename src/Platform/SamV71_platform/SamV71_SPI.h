#pragma once

#include "Abstract_SPI.h"

extern "C" {
#include "FreeRTOS.h"
#include "queue.h"
}

namespace SamV71 {
    class SamV71_SPI : public Abstract::Abstract_SPI {
        enum { Queue_size = 64 };

        StaticQueue_t the_queue_structure{};
        uint32_t      the_queue_storage[Queue_size]{};
        QueueHandle_t optional_queue{};

    public:
        SamV71_SPI() {
            optional_queue = xQueueCreateStatic(Queue_size, sizeof(void *), reinterpret_cast<uint8_t *>(the_queue_storage), &the_queue_structure);
            configASSERT(optional_queue != nullptr);
        }

        void initialize() override;

        bool transfer(Abstract::SPI_request *request) override;
    };
} // SamV71
