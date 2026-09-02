//
// Created by anolsen on 18.09.2019.
//

#include <samv71q21b.h>
#include <cortex_m4_definitions.h>
#include <component/spi.h>
#include <nvic_helpers.h>
#include "isr_spi.h"

static isr_spi_callbacks callbacks_Spi0{};

struct isr_spi_counters_t {
    int total_count;
    int rx_more;
    int rx_done;
    int tx_more;
    int tx_done;
    int rx_ready;
    int tx_ready;
    int overrun_error;
    int underrun_error;
    int tx_empty;
    int nss_raising;
    int mode_fault_error;
};

static isr_spi_counters_t isr_spi_counters{};

void handle_Spi_IRQ(Spi *p_Spi, const isr_spi_callbacks &callbacks);

extern "C" void ISR_SPI0() {
    handle_Spi_IRQ((Spi *) SPI0, callbacks_Spi0);
}

void handle_Spi_IRQ(Spi *p_Spi, const isr_spi_callbacks &callbacks) {
    uint32_t interrupt_mask = p_Spi->SPI_IMR;
    uint32_t channel_status = p_Spi->SPI_SR;
    isr_spi_counters.total_count++;
    if ((interrupt_mask & SPI_IMR_RDRF)) {
        uint8_t data = SPI_RDR_RD_Msk & p_Spi->SPI_RDR;
        if (callbacks.rx(callbacks.p_user, data)) {
            isr_spi_counters.rx_more++;
        } else {
            isr_spi_counters.rx_done++;
            p_Spi->SPI_IDR = (SPI_IDR_RDRF);
        }
    }
    if (interrupt_mask & (SPI_IMR_TDRE)) {
        uint8_t data;
        if (callbacks.tx(callbacks.p_user, &data)) {
            isr_spi_counters.tx_more++;
            p_Spi->SPI_TDR = SPI_TDR_TD(data);
        } else {
            isr_spi_counters.tx_done++;
            p_Spi->SPI_IDR = (SPI_IDR_TDRE);
        }
    }
    if (channel_status != 0) {
        if (channel_status & (SPI_SR_RDRF)) {
            isr_spi_counters.rx_ready++;
        }
        if (channel_status & (SPI_SR_TDRE)) {
            isr_spi_counters.tx_ready++;
        }
        if (channel_status & (SPI_SR_OVRES)) {
            isr_spi_counters.overrun_error++;
        }
        if (channel_status & (SPI_SR_UNDES)) {
            isr_spi_counters.underrun_error++;
        }
        if (channel_status & (SPI_SR_TXEMPTY)) {
            isr_spi_counters.tx_empty++;
        }
        if (channel_status & (SPI_SR_NSSR)) {
            isr_spi_counters.nss_raising++;
        }
        if (channel_status & (SPI_SR_MODF)) {
            isr_spi_counters.mode_fault_error++;
        }
    }
}

void register_SPI_interrupt_handler(uint32_t irq_number, const isr_spi_callbacks &callbacks) {
    NVIC_DisableIRQ(irq_number);
    if (SPI0_IRQn == irq_number) {
        callbacks_Spi0 = callbacks;
    }
    NVIC_ClearPendingIRQ(irq_number);
    NVIC_EnableIRQ(irq_number);
}

void enable_SPI_interrupts(uint32_t irq_number) {
    NVIC_DisableIRQ(irq_number);
    if (SPI0_IRQn == irq_number) {
        ((Spi *) SPI0)->SPI_IER = (
                SPI_IER_RDRF |
                SPI_IER_TDRE
        );
    }
    NVIC_ClearPendingIRQ(irq_number);
    NVIC_EnableIRQ(irq_number);
}

void disable_SPI_interrupts(uint32_t irq_number) {
    NVIC_DisableIRQ(irq_number);
    if (SPI0_IRQn == irq_number) {
        ((Spi *) SPI0)->SPI_IDR = (
                SPI_IDR_RDRF |
                SPI_IDR_TDRE |
                SPI_IDR_MODF |
                SPI_IDR_OVRES |
                SPI_IDR_NSSR |
                SPI_IDR_TXEMPTY |
                SPI_IDR_UNDES
        );
    }
    NVIC_ClearPendingIRQ(irq_number);
    NVIC_EnableIRQ(irq_number);
}
