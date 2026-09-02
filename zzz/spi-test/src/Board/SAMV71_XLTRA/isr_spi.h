//
// Created by anolsen on 18.09.2019.
//

#ifndef SPI_TEST_ISR_SPI_H
#define SPI_TEST_ISR_SPI_H

#ifdef __cplusplus

typedef bool (*isr_spi_rx)(void *p_user, uint8_t data);

typedef bool (*isr_spi_tx)(void *p_user, uint8_t *data);

typedef void (*isr_spi_tx_empty)(void *p_user);

typedef void (*isr_spi_error)(void *p_user, uint32_t diag);

struct isr_spi_callbacks {
    isr_spi_rx rx;
    isr_spi_tx tx;
    isr_spi_tx_empty tx_empty;
    isr_spi_error error;
    void *p_user;
};

void register_SPI_interrupt_handler(uint32_t irq_number, const isr_spi_callbacks &callbacks);

void enable_SPI_interrupts(uint32_t irq_number);

void disable_SPI_interrupts(uint32_t irq_number);

#else

void ISR_SPI0(void);

#endif

#endif //SPI_TEST_ISR_SPI_H
