//
// Created by anolsen on 16.09.2019.
//
#include <cstdint>
#include <cstring>
#include <command_parser.h>
#include "Labio.h"
#include "New_CLI.h"
#include "Consol.h"
#include "Command_handler.h"

const char *default_command = "help";

Labio::Labio(Consol &consol, Command_handler &handler) :
        mutex_lock(0),
        m_consol(consol),
        r_handler(handler),
        escape_received(0),
        buffer_pointer(0) {
    command_parse_result.action = NONE;
    memcpy(previous_command, default_command, 1 + strlen(default_command));
}

static void my_receiver(void *p_user, uint8_t data) {
    ((Labio *) p_user)->receive(data);
}

void Labio::receive(uint8_t data) {
    if (command_is_complete(data)) {
        memcpy(previous_command, command_buffer, Command_buffer_size);
        if ('$' == command_buffer[0]) {
            add_command_to_buffer(reinterpret_cast<char *>(command_buffer));
        } else if (mutex_try_lock()) {
            New_CLI::parse_commandline(reinterpret_cast<char *>(command_buffer), &command_parse_result);
            mutex_unlock();
        }
        buffer_pointer = 0;
    }
}

bool Labio::receive_input() {
    if (m_consol.for_each_input(this, my_receiver)) {
        return true;
    } else {
        return (0 < buffer_pointer);
    }
}

bool Labio::execute_pending_commands() {
    bool executed = true;
    if (get_num_unexecuted_commands()) {
        execute_next_command();
    } else if (mutex_try_lock()) {
        New_CLI::parse_result result{};
        memcpy((void *) &result, &command_parse_result, sizeof(result));
        command_parse_result.action = NONE;
        mutex_unlock();
        switch (result.action) {
            case NONE:
                executed = false;
                break;
            case HELP:
                New_CLI::print_help();
                break;
            default:
                r_handler.execute_command(result);
                break;
        }
    } else {
        executed = false;
    }
    return executed;
}

bool Labio::command_is_complete(uint8_t data) {
    bool complete = false;
    if (Command_buffer_size > buffer_pointer) {
        if(0 < escape_received){
            escape_received--;
        }
        else if (0 == buffer_pointer) {
            if ((' ' <= data) && ('~' >= data)) {
                m_consol.print("\r");
                m_consol.put(data);
                command_buffer[buffer_pointer++] = data;
            } else if (0x1Bu == data) {
                escape_received = 2;
                memcpy(command_buffer, previous_command, Command_buffer_size);
                buffer_pointer = strlen(command_buffer);
                m_consol.print("\r");
                m_consol.print(command_buffer);
            } else {
                m_consol.print("\r \r\n");
            }
        } else {
            if ((' ' <= data) && ('~' >= data)) {
                m_consol.put(data);
                command_buffer[buffer_pointer++] = data;
            } else if (0x8u == data) {
                m_consol.print("\b \b");
                buffer_pointer--;
            } else if ((0xAu == data) || (0xDu == data)) {
                m_consol.print("\r\n");
                command_buffer[buffer_pointer++] = 0x0u;
                complete = true;
            }
        }
    } else {
        m_consol.print("\r\nSorry, no such command.\r\n");
        buffer_pointer = 0;
    }
    return complete;
}

bool Labio::mutex_try_lock() {
    mutex_lock++;
    if (1 == mutex_lock) {
        return true;
    } else {
        mutex_lock--;
        return false;
    }
}

void Labio::mutex_unlock() {
    mutex_lock--;
}

