//
// Created by anolsen on 18.09.2019.
//

#ifndef NORM_FW_SAMV71_SPI_INTERRUPT_H
#define NORM_FW_SAMV71_SPI_INTERRUPT_H

#include <SPI_interface.h>

class SPI_interrupt {
public:
    typedef void (*interrupt_handler_t)(void *p_user);

    struct Interrupt_handler {
        uint32_t irq;
        void *p_user;
        interrupt_handler_t p_handler;
    };

    static void on_interrupt(uint32_t handler_number);

    static void register_handler(uint32_t handler_number, const Interrupt_handler &handler);

private:
    enum {
        Number_of_handlers = 1
    };
    static Interrupt_handler interrupt_handlers[Number_of_handlers];
};

void SPI0_handler();

#endif //NORM_FW_SAMV71_SPI_INTERRUPT_H
