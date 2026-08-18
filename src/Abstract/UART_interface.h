//
// Created by anolsen on 23.08.2019.
//
#ifndef NORM_FW_UART_INTERFACE_H
#define NORM_FW_UART_INTERFACE_H

#include <Utility/Utility_types.h>
#include <Utility/Ringbuffer.h>

/// Purpose: Hardware abstraction of the MCU UART peripheral.
class UART_interface {
public:
    UART_interface() = default;

    virtual void initialize(unsigned long bitrate) = 0;

    virtual bool has_input() = 0;

    virtual bool for_each_input(Optional_user, Optional_func) = 0;

    virtual bool is_ready() = 0;

    virtual int print(const char *ptr, int len) = 0;

    virtual int put(uint8_t c) = 0;

    virtual bool get(uint8_t *p_data) = 0;

    virtual void disable() {}

};

#endif //NORM_FW_UART_INTERFACE_H