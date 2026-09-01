//
// Created by anolsen on 18.09.2019.
//

#include <stdio.h>

#include <samv71q21b.h>
#include <samv71q21b_symbols.h>
#include <cortex_m7_definitions.h>
#include <component/spi.h>
#include <nvic_helpers.h>
#include "SPI_interrupt.h"

void SPI0_handler() { SPI_interrupt::on_interrupt(0); }

SPI_interrupt::Interrupt_handler SPI_interrupt::interrupt_handlers[]{};

void SPI_interrupt::on_interrupt(uint32_t handler_number) {
    if (handler_number < Number_of_handlers) {
        Interrupt_handler &handler = interrupt_handlers[handler_number];
        if (handler.p_handler && handler.irq) {
            NVIC_DisableIRQ(handler.irq);
            handler.p_handler(handler.p_user);
            NVIC_EnableIRQ(handler.irq);
        }
    }
}

void SPI_interrupt::register_handler(uint32_t handler_number, const SPI_interrupt::Interrupt_handler &handler) {
    if (handler_number < Number_of_handlers) {
        NVIC_DisableIRQ(handler.irq);
        interrupt_handlers[handler_number] = handler;
        NVIC_ClearPendingIRQ(handler.irq);
        NVIC_EnableIRQ(handler.irq);
    }
}

