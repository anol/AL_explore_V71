//
// Created by anolsen on 05.09.2019.
//
#pragma once

#include "Abstract_UART.h"

namespace Platform {
    class Common_stdio {
        Abstract_UART *optional_UART;

    public:
        explicit Common_stdio(Abstract_UART *UART) : optional_UART(UART) {
        }

        void initialize() const;
    };
}
