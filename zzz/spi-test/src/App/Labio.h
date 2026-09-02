//
// Created by anolsen on 16.09.2019.
//

#ifndef UART_TEST_LABIO_H
#define UART_TEST_LABIO_H

#include "New_CLI.h"

class Consol;
class Command_handler;

class Labio {
    enum {
        Command_buffer_size = 30
    };

public:
    Labio(Consol &consol, Command_handler &handler);

    bool execute_pending_commands();

    void receive(uint8_t data);

    bool receive_input();

private:
    bool command_is_complete(uint8_t data);

    bool mutex_try_lock();

    void mutex_unlock();

private:
    volatile int mutex_lock;
    Consol &m_consol;
    Command_handler &r_handler;
    int escape_received;
    int buffer_pointer;
    New_CLI::parse_result command_parse_result{};
    char command_buffer[Command_buffer_size];
    char previous_command[Command_buffer_size];
};

#endif //UART_TEST_LABIO_H
