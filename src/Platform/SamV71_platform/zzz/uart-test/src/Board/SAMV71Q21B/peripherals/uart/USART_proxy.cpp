//
// Created by anolsen on 23.08.2019.
//

#include <status_code.h>
#include "samv71q21b.h"
#include "cortex_m4_definitions.h"
#include "component/pmc.h"
#include "sysclk.h"
#include "nvic_helpers.h"
#include "isr_serial.h"
#include "ioport.h"
#include "USART_proxy.h"
#include "Assert_utility.h"

#define CONSOLE_BITRATE (115200UL)
/* The receiver sampling divide of baudrate clock. */
#define HIGH_FRQ_SAMPLE_DIV           16
#define LOW_FRQ_SAMPLE_DIV            8
/* The CD value scope programmed in MR register. */
#define MIN_CD_VALUE                  0x01
#define MAX_CD_VALUE                  US_BRGR_CD_Msk

USART_proxy::USART_proxy(const Usart_definition &definition) : m_definition(definition) {}

void USART_proxy::initialize() {
    pmc_enable_periph_clk(m_definition.USART_number);
    sysclk_enable_peripheral_clock(m_definition.USART_number);
    init_rs232(sysclk_get_peripheral_hz());
    init_interrupts();
}

bool USART_proxy::has_input() {
    return 0 < m_rx_buffer.num();
}

int USART_proxy::for_each_input(void *p_user, receiver_t p_receiver) {
    return m_rx_buffer.for_each(p_user, p_receiver);
}

int USART_proxy::print(const char *ptr, int len) {
    int nChars = 0;
    for (; len != 0; --len) {
        if (put(*ptr++) < 0) {
            return -1;
        }
        ++nChars;
    }
    return nChars;
}

static uint32_t my_write(Usart *p_USART, uint32_t data) {
    if (!(p_USART->US_CSR & US_CSR_TXRDY)) {
        return 1;
    }
    p_USART->US_THR = US_THR_TXCHR(data);
    return 0;
}

int USART_proxy::put(char c) {
    special_assert(0 != m_definition.p_USART);
    const bool non_blocking = true;
    if (non_blocking) {
        bool success = m_tx_buffer.put(c);
        m_definition.p_USART->US_IER = (US_IER_TXRDY | US_IER_TXEMPTY);
        return success ? STATUS_OK : ERR_IO_ERROR;
    } else {
        while (my_write(m_definition.p_USART, c));
        return STATUS_OK;
    }
}

static void isr_rx(void *p_user, uint8_t data) {
    if (p_user) {
        ((USART_proxy *) p_user)->rx(data);
    }
}

void USART_proxy::rx(uint8_t data) {
    m_rx_buffer.put(data);
}

static bool isr_tx(void *p_user, uint8_t *p_data) {
    if (p_user && p_data) {
        return ((USART_proxy *) p_user)->tx(p_data);
    } else {
        return false;
    }
}

bool USART_proxy::tx(uint8_t *p_data) {
    return m_tx_buffer.get(p_data);
}

static void isr_error(void *p_user, uint32_t diag) {
    if (p_user) {
        ((USART_proxy *) p_user)->error(diag);
    }
}

void USART_proxy::error(uint32_t diag) {
    // TODO: The very first read will cause an OVRE
//    while (true);
}

void USART_proxy::init_interrupts() {
    isr_serial_callbacks callbacks = {m_definition.irq_number, isr_rx, isr_tx, isr_error, (void *) this};
    NVIC_DisableIRQ(m_definition.irq_number);
    NVIC_ClearPendingIRQ(m_definition.irq_number);
    register_ISR_serial_handler(callbacks);
    NVIC_EnableIRQ(m_definition.irq_number);
}

void USART_proxy::reset() {
    m_definition.p_USART->US_WPMR = US_WPMR_WPKEY_PASSWD;
    m_definition.p_USART->US_MR = 0;
    m_definition.p_USART->US_RTOR = 0;
    m_definition.p_USART->US_TTGR = 0;
    m_definition.p_USART->US_CR = US_CR_RSTRX | US_CR_RXDIS;
    m_definition.p_USART->US_CR = US_CR_RSTTX | US_CR_TXDIS;
    m_definition.p_USART->US_CR = US_CR_RSTSTA;
    m_definition.p_USART->US_CR = US_CR_RTSDIS;
}

uint32_t USART_proxy::init_rs232(uint32_t ul_mck) {
    static uint32_t ul_reg_val;
    reset();
    ul_reg_val = 0;
    if (set_bitrate(CONSOLE_BITRATE, ul_mck)) {
        return 1;
    }
    ul_reg_val |= US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_CHMODE_NORMAL | US_MR_NBSTOP_1_BIT;
    ul_reg_val |= US_MR_USART_MODE_NORMAL;
    m_definition.p_USART->US_MR |= ul_reg_val;
    ioport_set_pin_mode(m_definition.rx_pin, m_definition.rx_mode);
    ioport_disable_pin(m_definition.rx_pin);
    ioport_set_pin_mode(m_definition.tx_pin, m_definition.tx_mode);
    ioport_disable_pin(m_definition.tx_pin);
    m_definition.p_USART->US_CR = US_CR_RXEN;
    m_definition.p_USART->US_CR = US_CR_TXEN;
    return 0;
}

uint32_t USART_proxy::set_bitrate(uint32_t bitrate, uint32_t ul_mck) {
    uint32_t over;
    uint32_t cd_fp;
    uint32_t cd;
    uint32_t fp;
    /* Calculate the receiver sampling divide of baudrate clock. */
    if (ul_mck >= HIGH_FRQ_SAMPLE_DIV * bitrate) {
        over = HIGH_FRQ_SAMPLE_DIV;
    } else {
        over = LOW_FRQ_SAMPLE_DIV;
    }
    /* Calculate clock divider according to the fraction calculated formula. */
    cd_fp = (8 * ul_mck + (over * bitrate) / 2) / (over * bitrate);
    cd = cd_fp >> 3u;
    fp = cd_fp & 0x07u;
    if (cd < MIN_CD_VALUE || cd > MAX_CD_VALUE) {
        return 1;
    }
    /* Configure the OVER bit in MR register. */
    if (over == 8) {
        m_definition.p_USART->US_MR |= US_MR_OVER;
    }
    /* Configure the baudrate generate register. */
    m_definition.p_USART->US_BRGR = (cd << US_BRGR_CD_Pos) | (fp << US_BRGR_FP_Pos);
    return 0;
}
