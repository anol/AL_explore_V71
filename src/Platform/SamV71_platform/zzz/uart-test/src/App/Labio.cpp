//
// Created by anolsen on 16.09.2019.
//
#include <cstdint>
#include <cstring>
#include <command_parser.h>
#include "Labio.h"

Labio::Labio(Consol &consol) : m_consol(consol), buffer_pointer(0) {
}

static void my_receiver(void *p_user, uint8_t data) {
    ((Labio *) p_user)->receive(data);
}

void Labio::receive(uint8_t data) {
    if (command_is_complete(data)) {
        add_command_to_buffer(reinterpret_cast<char *>(command_buffer));
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
    if (get_num_unexecuted_commands()) {
        execute_next_command();
        return true;
    }
    return false;
}

bool Labio::command_is_complete(uint8_t data) {
    bool complete = false;
    if (Command_buffer_size > buffer_pointer) {
        if (0 == buffer_pointer) {
            if ((' ' <= data) && ('~' >= data)) {
                m_consol.print("\r");
                m_consol.put(data);
                command_buffer[buffer_pointer++] = data;
            } else {
                m_consol.print("\r \r\n");
            }
        } else {
            if ((' ' <= data) && ('~' >= data)) {
                m_consol.put(data);
                command_buffer[buffer_pointer++] = data;
            } else if ((0xAu == data) || (0xDu == data)) {
                if ('$' == command_buffer[0]) {
                    m_consol.print("\r\n");
                    command_buffer[buffer_pointer++] = 0x0u;
                    complete = true;
                } else {
                    m_consol.print("\r\nSorry, but commands must start with a '$'-symbol.\r\n");
                    buffer_pointer = 0;
                }
            }
        }
    } else {
        m_consol.print("\r\nSorry, no such command.\r\n");
        buffer_pointer = 0;
    }
    return complete;
}

