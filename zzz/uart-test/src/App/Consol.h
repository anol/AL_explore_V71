//
// Created by anolsen on 22.08.2019.
//

#ifndef PM_V71_CONSOL_UART_H
#define PM_V71_CONSOL_UART_H


#include "USART_proxy.h"

class Consol {

public:
    Consol();

    void initialize(const char *header);

    void put(uint8_t data);

    void print(const char *string);

    void print_idler();

    void check_input();

    void receive(uint8_t data);

    bool for_each_input(void *p_user, receiver_t p_receiver);

    static void global_printf(const char *format, ...);

private:
    USART_proxy proxy;

    int m_progress;
};


#endif //PM_V71_CONSOL_UART_H
