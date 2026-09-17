#pragma once

#include "Abstract_SPI.h"
#include "Transfer_request.h"
#include "FreeRTOS_queue.h"

namespace SamV71 {
    class SamV71_SPI : public Abstract::Abstract_SPI {
        enum { Queue_size = 64 };

        FreeRTOS::FreeRTOS_queue<void *, Queue_size> the_queue{};

    public:
        void initialize() override;

        bool transfer(Generic::Transfer_request *request) override;
    };
} // SamV71
