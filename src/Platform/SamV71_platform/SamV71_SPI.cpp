#include "SamV71_SPI.h"

namespace SamV71 {
    void SamV71_SPI::initialize() {
    }

    bool SamV71_SPI::transfer(Generic::Transfer_request *request) {
        bool success{};
        if (request && request->get_semaphore()) {
            if (the_queue.send(request)) {
                auto *semaphore = request->get_semaphore();
                semaphore->take();
                success = true;
            }
        }
        return success;
    }
} // SamV71
