#pragma once

#include "Abstract_request.h"

namespace Abstract {
    class Abstract_SPI {
    public:
        virtual ~Abstract_SPI() = default;

        virtual void initialize() = 0;

        /**
          * @name transfer
          * @param request A pointer to a transfer reqeust instance.
          * @return true=success, false=failed.
          **/
        virtual bool transfer(Abstract_request *request) = 0;
    };
} // Abstract
