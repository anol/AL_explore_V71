//
// Created by anolsen on 05.09.2019.
//
#pragma once

#include "Abstract_UART.h"

class Console_task;

class SamV71_stdio {
public:
    static void construct(Abstract_UART &);
};
