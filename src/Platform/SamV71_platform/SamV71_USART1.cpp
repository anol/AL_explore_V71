//
// Created by aeols on 2026-08-24.
//

#include "SamV71_USART1.h"
#include "sam.h"
#include "SamV71_clock.h"

namespace SamV71
{
    SamV71_USART1* SamV71_USART1::optional_one_and_only_UART{};
    uint32_t SamV71_USART1::the_USART1_IRQ_count{};
    uint32_t SamV71_USART1::the_USART1_IRQ_status{};
    uint32_t SamV71_USART1::the_USART1_TX_count{};
    uint32_t SamV71_USART1::the_USART1_RX_count{};
}

extern "C" void USART1_ISR(void)
{
    using namespace SamV71;
    SamV71_USART1::the_USART1_IRQ_count =  SamV71_USART1::the_USART1_IRQ_count + 1;
    SamV71_USART1::the_USART1_IRQ_status = USART1_REGS->US_CSR;
    /* Error status */
    uint32_t errorStatus = (SamV71_USART1::the_USART1_IRQ_status & (US_CSR_USART_OVRE_Msk | US_CSR_USART_FRAME_Msk |
        US_CSR_USART_PARE_Msk));
    if (errorStatus != 0)
    {
        /* Client must call USARTx_ErrorGet() function to clear the errors */
        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART1_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk |
            US_IDR_USART_OVRE_Msk);
        /* USART errors are normally associated with the receiver, hence calling receiver callback */
        // if (usart1Obj.rdCallback != NULL) {
        //     usart1Obj.rdCallback(USART_EVENT_READ_ERROR, usart1Obj.rdContext);
        // }
    }
    if (SamV71_USART1::optional_one_and_only_UART)
    {
        /* Receiver status */
        if (SamV71_USART1::the_USART1_IRQ_status & US_CSR_USART_RXRDY_Msk)
        {
            SamV71_USART1::optional_one_and_only_UART->on_receiver_interrupt();
        }
        /* Transmitter status */
        if (SamV71_USART1::the_USART1_IRQ_status & (US_CSR_USART_TXRDY_Msk | US_CSR_USART_TXEMPTY_Msk))
        {
            SamV71_USART1::optional_one_and_only_UART->on_transmitter_interrupt();
        }
    }
}

namespace SamV71
{
    void SamV71_USART1::initialize()
    {
        the_RX_queue.initialize();
        the_TX_queue.initialize();
        SamV71_clock::enable_peripheral_clock(USART1_INSTANCE_ID);
        optional_one_and_only_UART = this;
        USART1_REGS->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk); // Reset UART
        USART1_REGS->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk); // Enable UART
        USART1_REGS->US_MR = // Set UART mode
            US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | (0 <<
                US_MR_USART_OVER_Pos);
        USART1_REGS->US_BRGR = US_BRGR_CD(81); // Set bitrate
        NVIC_DisableIRQ(USART1_IRQn);
        NVIC_ClearPendingIRQ(USART1_IRQn);
        NVIC_EnableIRQ(USART1_IRQn);
        enable_receiver_interrupt();
    }

    void SamV71_USART1::enable_receiver_interrupt()
    {
        USART1_REGS->US_IER = (US_IER_USART_RXRDY_Msk | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk |
            US_IER_USART_OVRE_Msk);
    }

    void SamV71_USART1::disable_receiver_interrupt()
    {
        USART1_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk |
            US_IDR_USART_OVRE_Msk);
    }

    void SamV71_USART1::enable_transmitter_interrupt()
    {
        USART1_REGS->US_IER = US_IER_USART_TXEMPTY_Msk | US_IER_USART_TXRDY_Msk;
    }

    void SamV71_USART1::disable_transmitter_interrupt()
    {
        USART1_REGS->US_IDR = US_IDR_USART_TXEMPTY_Msk | US_IDR_USART_TXRDY_Msk;
    }

    void SamV71_USART1::on_receiver_interrupt()
    {
        while (USART1_REGS->US_CSR & US_CSR_USART_RXRDY_Msk)
        {
            const auto data = USART1_REGS->US_RHR;
            the_RX_queue.put(static_cast<uint8_t>(data & 0xFF));
            the_USART1_RX_count = the_USART1_RX_count + 1;
        }
    }

    void SamV71_USART1::on_transmitter_interrupt()
    {
        uint8_t data;
        while (USART1_REGS->US_CSR & (US_CSR_USART_TXRDY_Msk | US_CSR_USART_TXEMPTY_Msk))
        {
            if (the_TX_queue.get(&data))
            {
                USART1_REGS->US_THR = data;
                the_USART1_TX_count = the_USART1_TX_count + 1;
            }
            else
            {
                disable_transmitter_interrupt();
                break;
            }
        }
    }

    int SamV71_USART1::print(const char* data, int len)
    {
        int written = 0;
        disable_transmitter_interrupt();
        while (len > 0)
        {
            if (the_TX_queue.put(*data++))
            {
                written++;
            }
            else
            {
                break;
            }
        }
        on_transmitter_interrupt();
        enable_transmitter_interrupt();
        return written;
    }
} // SamV71
