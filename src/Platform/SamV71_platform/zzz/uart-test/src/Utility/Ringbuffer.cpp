//
// Created by anolsen on 12.09.2019.
//
#include <stdint.h>
#include "status_code.h"
#include "Ringbuffer.h"
#include "Assert_utility.h"

// TODO: Fix this ringbuffer to handle wrapping!

Ringbuffer::Ringbuffer() :
        read_index(0),
        write_index(0) {}

bool Ringbuffer::get(uint8_t *data) {
    special_assert(read_index < Buffer_size);
    if (write_index != read_index) {
        *data = buf[read_index++];
        if (read_index >= Buffer_size) {
            read_index = 0;
        }
        return true;
    } else {
        return false;
    }
}

bool Ringbuffer::put(uint8_t data) {
    special_assert(write_index < Buffer_size);
    buf[write_index++] = data;
    if (write_index >= Buffer_size) {
        write_index = 0;
    }
    return true;
}

uint32_t Ringbuffer::num() {
    if (write_index >= read_index) {
        return write_index - read_index;
    } else {
        return (Buffer_size + write_index) - read_index;
    }
}

int Ringbuffer::for_each(void *p_user, receiver_t p_receiver) {
    int count = 0;
    uint8_t data;
    while (get(&data)) {
        count++;
        p_receiver(p_user, data);
    }
    return count;
}
