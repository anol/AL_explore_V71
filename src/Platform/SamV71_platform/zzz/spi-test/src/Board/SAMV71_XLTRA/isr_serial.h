//
// Created by anolsen on 11.09.2019.
//

#ifndef UART_TEST_ISR_SERIAL_H
#define UART_TEST_ISR_SERIAL_H

#ifdef __cplusplus

typedef void (*isr_serial_rx)(void *p_user, uint8_t data);

typedef bool (*isr_serial_tx)(void *p_user, uint8_t *data);

typedef void (*isr_serial_error)(void *p_user, uint32_t diag);

struct isr_serial_callbacks {
    uint32_t irq;
    isr_serial_rx rx;
    isr_serial_tx tx;
    isr_serial_error error;
    void *p_user;
};

void register_ISR_serial_handler(const isr_serial_callbacks &callbacks);

#else

void ISR_USART1(void);

#endif

#endif //UART_TEST_ISR_SERIAL_H
