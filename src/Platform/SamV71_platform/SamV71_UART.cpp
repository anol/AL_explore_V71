//
// Created by aeols on 2026-08-24.
//

#include "SamV71_UART.h"
#include "sam.h"

SamV71::SamV71_UART *optional_one_and_only_UART{};

extern "C" void USART1_InterruptHandler(void) {
    /* Error status */
    uint32_t errorStatus = (USART1_REGS->US_CSR & (US_CSR_USART_OVRE_Msk | US_CSR_USART_FRAME_Msk | US_CSR_USART_PARE_Msk));
    if (errorStatus != 0) {
        /* Client must call USARTx_ErrorGet() function to clear the errors */
        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART1_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);
        /* USART errors are normally associated with the receiver, hence calling receiver callback */
        // if (usart1Obj.rdCallback != NULL) {
        //     usart1Obj.rdCallback(USART_EVENT_READ_ERROR, usart1Obj.rdContext);
        // }
    }
    if (optional_one_and_only_UART != nullptr) {
        /* Receiver status */
        if (US_CSR_USART_RXRDY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_RXRDY_Msk)) {
            optional_one_and_only_UART->ISR_on_receive();
        }
        /* Transmitter status */
        if (US_CSR_USART_TXRDY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_TXRDY_Msk)) {
            optional_one_and_only_UART->ISR_on_transmit();
        }
    }
}

namespace SamV71 {
    void SamV71_UART::initialize() {
        optional_one_and_only_UART = this;
        /* Reset USART1 */
        USART1_REGS->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk);
        /* Enable USART1 */
        USART1_REGS->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);
        /* Configure USART1 mode */
        USART1_REGS->US_MR = (US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | (0 << US_MR_USART_OVER_Pos));
        /* Configure USART1 Baud Rate */
        USART1_REGS->US_BRGR = US_BRGR_CD(81);
        /* Enable Read, Overrun, Parity and Framing error interrupts */
        enable_receiver_interrupt();
    }

    void SamV71_UART::enable_receiver_interrupt() {
        USART1_REGS->US_IER = (US_IER_USART_RXRDY_Msk | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk | US_IER_USART_OVRE_Msk);
    }

    void SamV71_UART::disable_receiver_interrupt() {
        USART1_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);
    }

    void SamV71_UART::enable_transmitter_interrupt() {
        USART1_REGS->US_IER = US_IER_USART_TXEMPTY_Msk;
    }

    void SamV71_UART::disable_transmitter_interrupt() {
        USART1_REGS->US_IDR = US_IDR_USART_TXEMPTY_Msk;
    }

    void SamV71_UART::ISR_on_receive() {
        /* Keep reading until there is a character availabe in the RX FIFO */
        while (US_CSR_USART_RXRDY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_RXRDY_Msk)) {
            the_RX_queue.put((uint8_t) (USART1_REGS->US_RHR & US_RHR_RXCHR_Msk));
        }
    }

    void SamV71_UART::ISR_on_transmit() {
        uint8_t data;
        /* Keep writing to the TX FIFO as long as there is space */
        while (US_CSR_USART_TXEMPTY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_TXEMPTY_Msk)) {
            if (the_TX_queue.get(&data)) {
                USART1_REGS->US_THR |= data;
            } else {
                /* Nothing to transmit. Disable the data register empty interrupt. */
                disable_transmitter_interrupt();
                break;
            }
        }
    }

    int SamV71_UART::print(const char *data, int len) {
        int written = 0;
        disable_transmitter_interrupt();
        while (len > 0) {
            if (the_TX_queue.put(*data++)) {
                written++;
            } else {
                break;
            }
        }
        enable_transmitter_interrupt();
        return written;
    }
} // SamV71
