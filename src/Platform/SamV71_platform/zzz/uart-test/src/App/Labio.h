//
// Created by anolsen on 16.09.2019.
//

#ifndef UART_TEST_LABIO_H
#define UART_TEST_LABIO_H

#include "Consol.h"

class Labio {
    enum {
        Command_buffer_size = 30
    };

public:
    Labio(Consol &consol);

    bool execute_pending_commands();

    void receive(uint8_t data);

    Consol &m_consol;

    bool receive_input();

private:
    bool command_is_complete(uint8_t data);

private:
    int buffer_pointer;
    char command_buffer[Command_buffer_size];

};

#endif //UART_TEST_LABIO_H
