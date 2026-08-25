//
// Created by aeols on 2026-08-24.
//

#include "SamV71_UART.h"
#include "sam.h"


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
    /* Receiver status */
    if (US_CSR_USART_RXRDY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_RXRDY_Msk)) {
        SamV71::SamV71_UART::USART1_ISR_RX_Handler();
    }
    /* Transmitter status */
    if (US_CSR_USART_TXRDY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_TXRDY_Msk)) {
        SamV71::SamV71_UART::USART1_ISR_TX_Handler();
    }
}

namespace SamV71 {
    void SamV71_UART::initialize() {
        /* Reset USART1 */
        USART1_REGS->US_CR   = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk);
        /* Enable USART1 */
        USART1_REGS->US_CR   = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);
        /* Configure USART1 mode */
        USART1_REGS->US_MR   = (US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | (0 << US_MR_USART_OVER_Pos));
        /* Configure USART1 Baud Rate */
        USART1_REGS->US_BRGR = US_BRGR_CD(81);
        /* Enable Read, Overrun, Parity and Framing error interrupts */
        USART1_RX_INT_ENABLE();
    }

    void SamV71_UART::USART1_RX_INT_ENABLE() {
        USART1_REGS->US_IER = (US_IER_USART_RXRDY_Msk | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk | US_IER_USART_OVRE_Msk);
    }

    void SamV71_UART::USART1_RX_INT_DISABLE() {
        USART1_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);
    }

    void SamV71_UART::USART1_TX_INT_ENABLE() {
        USART1_REGS->US_IER = US_IER_USART_TXEMPTY_Msk;
    }

    void SamV71_UART::USART1_TX_INT_DISABLE() {
        USART1_REGS->US_IDR = US_IDR_USART_TXEMPTY_Msk;
    }


    void SamV71_UART::USART1_ISR_RX_Handler(void) {
        /* Keep reading until there is a character availabe in the RX FIFO */
        while (US_CSR_USART_RXRDY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_RXRDY_Msk)) {
            if (USART1_RxPushByte((uint8_t) (USART1_REGS->US_RHR & US_RHR_RXCHR_Msk)) == true) {
                USART1_ReadNotificationSend();
            }
        }
    }

    void SamV71_UART::USART1_ISR_TX_Handler(void) {
        uint8_t wrByte;
        /* Keep writing to the TX FIFO as long as there is space */
        while (US_CSR_USART_TXEMPTY_Msk == (USART1_REGS->US_CSR & US_CSR_USART_TXEMPTY_Msk)) {
            if (USART1_TxPullByte(&wrByte) == true) {
                USART1_REGS->US_THR |= wrByte;
                /* Send notification */
                USART1_WriteNotificationSend();
            } else {
                /* Nothing to transmit. Disable the data register empty interrupt. */
                USART1_TX_INT_DISABLE();
                break;
            }
        }
    }

    bool SamV71_UART::has_input() {
        return false;
    }

    bool SamV71_UART::for_each_input(Optional_user, Optional_func) {
        size_t nBytesRead = 0;
        uint32_t rdOutIndex;
        uint32_t rdInIndex;

        while (nBytesRead < size)
        {
            USART1_RX_INT_DISABLE();

            rdOutIndex = usart1Obj.rdOutIndex;
            rdInIndex = usart1Obj.rdInIndex;

            if (rdOutIndex != rdInIndex)
            {
                pRdBuffer[nBytesRead++] = USART1_ReadBuffer[usart1Obj.rdOutIndex++];

                if (usart1Obj.rdOutIndex >= USART1_READ_BUFFER_SIZE)
                {
                    usart1Obj.rdOutIndex = 0;
                }
                USART1_RX_INT_ENABLE();
            }
            else
            {
                USART1_RX_INT_ENABLE();
                break;
            }
        }

        return nBytesRead;
    }

    bool SamV71_UART::is_ready() {
        return false;
    }

    int SamV71_UART::print(const char *ptr, int len) {
        size_t nBytesWritten = 0;
        USART1_TX_INT_DISABLE();
        while (nBytesWritten < size) {
            if (USART1_TxPushByte(pWrBuffer[nBytesWritten]) == true) {
                nBytesWritten++;
            } else {
                /* Queue is full, exit the loop */
                break;
            }
        }
        /* Check if any data is pending for transmission */
        if (USART1_WritePendingBytesGet() > 0) {
            /* Enable TX interrupt as data is pending for transmission */
            USART1_TX_INT_ENABLE();
        }
        return nBytesWritten;
    }

    int SamV71_UART::put(uint8_t c) {
        return 0;
    }

    bool SamV71_UART::get(uint8_t *p_data) {
        return false;
    }
} // SamV71
