//
// Created by aeols on 2026-08-24.
//

module;
#include <cstdint>
#include "FreeRTOS.h"
#include "queue.h"
#include "sam.h"

module Platform.SamV71_USART1;
import Type.Abstract_UART;
import Type.Misc_type;
import Platform.SamV71_clock;

namespace SamV71 {
    SamV71_USART1 *SamV71_USART1::optional_one_and_only_UART{};
    uint32_t       SamV71_USART1::the_USART1_IRQ_count{};
    uint32_t       SamV71_USART1::the_USART1_IRQ_status{};
    uint32_t       SamV71_USART1::the_USART1_TX_count{};
    uint32_t       SamV71_USART1::the_USART1_TX_overflow_count{};
    uint32_t       SamV71_USART1::the_USART1_RX_count{};
    uint32_t       SamV71_USART1::the_USART1_RX_overflow_count{};
}

extern "C" void USART1_ISR(void) {
    using namespace SamV71;
    BaseType_t xHigherPriorityTaskWoken  = pdFALSE;
    SamV71_USART1::the_USART1_IRQ_count  = SamV71_USART1::the_USART1_IRQ_count + 1;
    SamV71_USART1::the_USART1_IRQ_status = USART1_REGS->US_CSR;
    /* Error status */
    uint32_t errorStatus                 = (SamV71_USART1::the_USART1_IRQ_status & (US_CSR_USART_OVRE_Msk | US_CSR_USART_FRAME_Msk |
                                                                    US_CSR_USART_PARE_Msk));
    if (errorStatus != 0) {
        // Disable Read, Overrun, Parity and Framing error interrupts
        USART1_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk |
                               US_IDR_USART_OVRE_Msk);
    }
    if (SamV71_USART1::optional_one_and_only_UART) {
        if (SamV71_USART1::the_USART1_IRQ_status & US_CSR_USART_RXRDY_Msk) {
            SamV71_USART1::optional_one_and_only_UART->on_receiver_interrupt(&xHigherPriorityTaskWoken);
        }
        if (SamV71_USART1::the_USART1_IRQ_status & (US_CSR_USART_TXRDY_Msk | US_CSR_USART_TXEMPTY_Msk)) {
            SamV71_USART1::optional_one_and_only_UART->on_transmitter_interrupt(&xHigherPriorityTaskWoken);
        }
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

namespace SamV71 {
    SamV71_USART1::SamV71_USART1() {
        optional_RX_queue = xQueueCreateStatic(RX_queue_size, sizeof(uint8_t), reinterpret_cast<uint8_t *>(the_RX_storage), &the_RX_structure);
        optional_TX_queue = xQueueCreateStatic(TX_queue_size, sizeof(uint8_t), reinterpret_cast<uint8_t *>(the_TX_storage), &the_TX_structure);
        configASSERT(optional_RX_queue != nullptr);
        configASSERT(optional_TX_queue != nullptr);
    }

    void SamV71_USART1::initialize() {
        init_USART1();
        init_interrupt();
    }

    void SamV71_USART1::init_USART1() {
        SamV71_clock::enable_peripheral_clock(USART1_INSTANCE_ID);
        USART1_REGS->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk); // Reset UART
        USART1_REGS->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);                            // Enable UART
        USART1_REGS->US_MR =                                                                           // Set UART mode
                US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | (0 <<
                    US_MR_USART_OVER_Pos);
        USART1_REGS->US_BRGR       = US_BRGR_CD(81); // Set bitrate
        optional_one_and_only_UART = this;
    }

    void SamV71_USART1::init_interrupt() {
        NVIC_DisableIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
        NVIC_ClearPendingIRQ(USART1_IRQn);
        NVIC_EnableIRQ(USART1_IRQn);
        enable_receiver_interrupt();
    }

    void SamV71_USART1::enable_receiver_interrupt() {
        USART1_REGS->US_IER = US_IER_USART_RXRDY_Msk;
        //  | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk | US_IER_USART_OVRE_Msk;
    }

    void SamV71_USART1::disable_receiver_interrupt() {
        USART1_REGS->US_IDR = US_IDR_USART_RXRDY_Msk;
        //  | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk;
    }

    void SamV71_USART1::enable_transmitter_interrupt() {
        USART1_REGS->US_IER = US_IER_USART_TXEMPTY_Msk | US_IER_USART_TXRDY_Msk;
    }

    void SamV71_USART1::disable_transmitter_interrupt() {
        USART1_REGS->US_IDR = US_IDR_USART_TXEMPTY_Msk | US_IDR_USART_TXRDY_Msk;
    }

    void SamV71_USART1::on_receiver_interrupt(BaseType_t *pxHigherPriorityTaskWoken) {
        while (USART1_REGS->US_CSR & US_CSR_USART_RXRDY_Msk) {
            const auto data = static_cast<uint8_t>(USART1_REGS->US_RHR & 0xFF);
            if (xQueueSendFromISR(optional_RX_queue, &data, pxHigherPriorityTaskWoken) == pdPASS) {
                the_USART1_RX_count = the_USART1_RX_count + 1;
            } else {
                the_USART1_RX_overflow_count = the_USART1_RX_overflow_count + 1;
            }
        }
    }

    void SamV71_USART1::on_transmitter_interrupt(BaseType_t *pxHigherPriorityTaskWoken) {
        uint8_t data;
        while (USART1_REGS->US_CSR & (US_CSR_USART_TXRDY_Msk | US_CSR_USART_TXEMPTY_Msk)) {
            if (xQueueReceiveFromISR(optional_TX_queue, &data, pxHigherPriorityTaskWoken) == pdPASS) {
                USART1_REGS->US_THR = data;
                the_USART1_TX_count = the_USART1_TX_count + 1;
            } else {
                disable_transmitter_interrupt();
                break;
            }
        }
    }

    bool SamV71_USART1::for_each_input(Optional_user user, Optional_func func) {
        uint8_t data;
        while (xQueueReceive(optional_RX_queue, &data, 0) == pdPASS) {
            if (func) {
                func(user, data);
            }
        }
        return true;
    }

    int SamV71_USART1::print(const char *data, int len) {
        int written = 0;
        while (len-- > 0) {
            if (xQueueSend(optional_TX_queue, data, 0) == pdPASS) {
                ++data;
                ++written;
            } else {
                the_USART1_TX_overflow_count = the_USART1_TX_overflow_count + 1;
                break;
            }
        }
        enable_transmitter_interrupt();
        return written;
    }
} // SamV71
