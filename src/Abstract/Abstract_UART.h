//
// Created by anolsen on 23.08.2019.
//

#pragma once


#include <Utility/Utility_types.h>
#include <Utility/Ringbuffer.h>

class Abstract_UART {
public:
    Abstract_UART() = default;

    virtual ~Abstract_UART() = default;

    virtual void initialize() = 0;

    virtual bool has_input() = 0;

    virtual bool for_each_input(Optional_user, Optional_func) = 0;

    virtual bool is_ready() = 0;

    virtual int print(const char *ptr, int len) = 0;

    virtual int put(uint8_t c) = 0;

    virtual bool get(uint8_t *p_data) = 0;
};
