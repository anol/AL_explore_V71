#include "SamV71_SPI.h"

namespace SamV71 {
    void SamV71_SPI::initialize() {
    }

    bool SamV71_SPI::transfer(Abstract::SPI_request *request) {
        bool success{};
        if (optional_queue) {
            auto result = xQueueSend(optional_queue, request, eNoTasksWaitingTimeout);
            success = result == pdPASS;
        }
        return success;
    }
} // SamV71
