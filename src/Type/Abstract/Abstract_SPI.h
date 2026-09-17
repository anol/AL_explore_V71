#pragma once

#include "Transfer_request.h"

namespace Abstract {
    class Abstract_SPI {
    public:
        virtual ~Abstract_SPI() = default;

        virtual void initialize() = 0;

        virtual bool transfer(Generic::Transfer_request *request) = 0;
    };
} // Abstract
