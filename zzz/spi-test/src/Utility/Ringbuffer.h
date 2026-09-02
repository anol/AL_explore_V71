//
// Created by anolsen on 12.09.2019.
//

#ifndef UART_TEST_RINGBUFFER_H
#define UART_TEST_RINGBUFFER_H

typedef void (*receiver_t)(void *p_user, uint8_t data);

class Ringbuffer {
    enum {
        Buffer_size = 10000
    };

public:
    Ringbuffer();

    bool get(uint8_t *data);

    bool put(uint8_t data);

    uint32_t num();

    int for_each(void *p_user, receiver_t p_receiver);

    bool is_getting_filled();

private:
    volatile uint32_t read_index;  /** Buffer read index */
    volatile uint32_t write_index; /** Buffer write index */
    volatile int buffer_filling;
    volatile uint8_t buf[Buffer_size];         /** Buffer base address */

};

#endif //UART_TEST_RINGBUFFER_H
