//
// Created by anolsen on 11.09.2019.
//

#include <samv71q21b.h>
#include <component/usart.h>
#include "isr_serial.h"

static isr_serial_callbacks callbacks_USART1 = {static_cast<IRQn_Type>(0)};

void handle_USART_IRQ(Usart *p_USART, const isr_serial_callbacks &callbacks);

extern "C" void ISR_USART1() {
    handle_USART_IRQ((Usart *) USART1, callbacks_USART1);
}

void handle_USART_IRQ(Usart *p_USART, const isr_serial_callbacks &callbacks) {
    uint32_t interrupt_mask = p_USART->US_IMR;
    uint32_t channel_status = p_USART->US_CSR;
    if (channel_status & (US_CSR_OVRE | US_CSR_FRAME | US_CSR_PARE | US_CSR_MANERR)) {
        p_USART->US_CR = US_CR_RSTSTA;
        volatile uint8_t data = 0xFu & p_USART->US_RHR;
        callbacks.error(callbacks.p_user, channel_status);
    }
    if (channel_status & US_CSR_RXRDY) {
        uint8_t data = US_RHR_RXCHR_Msk & p_USART->US_RHR;
        callbacks.rx(callbacks.p_user, data);
    }
    if (channel_status & (US_CSR_TXRDY | US_CSR_TXEMPTY)) {
        uint8_t data;
        if (callbacks.tx(callbacks.p_user, &data)) {
            p_USART->US_THR = US_THR_TXCHR(data);
        } else {
            p_USART->US_IDR = (US_CSR_TXRDY | US_CSR_TXEMPTY);
        }
    }
}

void register_ISR_serial_handler(const isr_serial_callbacks &callbacks) {
    switch (callbacks.irq) {
        case USART1_IRQn:
            callbacks_USART1 = callbacks;
            ((Usart *) USART1)->US_IER = (US_IER_RXRDY | US_IER_TXRDY | US_IER_TXEMPTY);
            break;
        default:;
    }
}
