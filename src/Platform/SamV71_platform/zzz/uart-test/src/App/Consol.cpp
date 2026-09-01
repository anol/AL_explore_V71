//
// Created by anolsen on 22.08.2019.
//

#include <cstdarg>
#include <cstring>
#include <stdio.h>
#include <Assert_utility.h>
#include "samv71q21b.h"
#include "samv71q21b_pio.h"
#include "ioport.h"
#include "board_definitions.h"
#include "Consol.h"

#define TX_BUFFER_LENGTH 1000


extern "C" {
//! Pointer to the base of the USART module instance to use for stdio.
volatile void *volatile stdio_base;
//! Pointer to the external low level write function.
int (*ptr_put)(void volatile *, char);
//! Pointer to the external low level read function.
void (*ptr_get)(void volatile *, char *);
}

static Consol *the_one_and_only_consol = nullptr;

const USART_proxy::Usart_definition definition = {
        USART1,
        ID_USART1,
        USART1_IRQn,
        USART1_RXD_GPIO,
        USART1_RXD_FLAGS,
        USART1_TXD_GPIO,
        USART1_TXD_FLAGS};

Consol::Consol() : proxy(definition), m_progress(0) {}

void Consol::initialize(const char *header) {
    proxy.initialize();
    the_one_and_only_consol = this;
    print(header);
}

void Consol::print(const char *string) {
    special_assert(this == the_one_and_only_consol);
    proxy.print(string, strlen(string));
}

void Consol::put(uint8_t data) {
    special_assert(this == the_one_and_only_consol);
    proxy.put(data);
}

void Consol::print_idler() {
    switch (m_progress++) {
        case 0:
            print("\r|");
            break;
        case 1:
            print("\r/");
            break;
        case 2:
            print("\r-");
            break;
        default:
            print("\r\\");
            m_progress = 0;
            break;
    }
}

static void my_receiver(void *p_user, uint8_t data) {
    ((Consol *) p_user)->receive(data);
}

void Consol::receive(uint8_t data) {
    proxy.put(data);
}

void Consol::check_input() {
    if (proxy.has_input()) {
        proxy.for_each_input(this, my_receiver);
    }
}

bool Consol::for_each_input(void *p_user, receiver_t p_receiver) {
    if (proxy.has_input()) {
        proxy.for_each_input(p_user, p_receiver);
        return true;
    } else {
        return false;
    }
}

void Consol::global_printf(const char *format, ...) {
    if (the_one_and_only_consol) {
        va_list args;
        static uint8_t temp_buf[TX_BUFFER_LENGTH];
        va_start(args, format);
        vsprintf((char *) temp_buf, format, args);
        va_end(args);
        the_one_and_only_consol->print(reinterpret_cast<const char *>(temp_buf));
    }
}

