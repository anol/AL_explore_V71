//
// Created by anolsen on 23.08.2019.
//

#include <cstdint>

#include "samv71q21b_symbols.h"
#include "component/uart/USART_interrupt.h"
#include "component/clock/SAMV71_clock.h"
#include "SAMV71_UART.h"

#define CONSOLE_BITRATE (115200UL)
/* The receiver sampling divide of baudrate clock. */
#define HIGH_FRQ_SAMPLE_DIV           16
#define LOW_FRQ_SAMPLE_DIV            8
/* The CD value scope programmed in MR register. */
#define MIN_CD_VALUE                  0x01
#define MAX_CD_VALUE                  US_BRGR_CD_Msk

SAMV71_UART::SAMV71_UART(const Usart_definition &definition) :
        m_definition(definition),
        rx_pin(definition.rx_port, definition.rx_pin, false, definition.rx_mode),
        tx_pin(definition.tx_port, definition.tx_pin, false, definition.tx_mode) {}

void SAMV71_UART::initialize(uint32_t bitrate) {
    setup_UART(CONSOLE_BITRATE);
    setup_interrupts();
}

bool SAMV71_UART::has_input() {
    return 0 < m_rx_buffer.get_fill();
}

bool SAMV71_UART::get(uint8_t *p_data) {
    return m_rx_buffer.get(p_data);
}

int SAMV71_UART::print(const char *ptr, int len) {
    int nChars = 0;
    for (; len != 0; --len) {
        if (put(*ptr++) < 0) {
            return nChars;
        }
        ++nChars;
    }
    return nChars;
}

int SAMV71_UART::put(uint8_t c) {
    if (m_tx_buffer.is_getting_filled()) {
        m_overflow_count++;
        return 0;
    }
    bool success = m_tx_buffer.put(c);
    m_definition.p_USART->US_IER = (US_IER_TXRDY | US_IER_TXEMPTY);
    return success ? 0 : -1;
}

static void isr_rx(void *p_user, uint8_t data) {
    if (p_user) {
        ((SAMV71_UART *) p_user)->rx(data);
    }
}

void SAMV71_UART::rx(uint8_t data) {
    m_rx_buffer.put(data);
}

static bool isr_tx(void *p_user, uint8_t *p_data) {
    if (p_user && p_data) {
        return ((SAMV71_UART *) p_user)->tx(p_data);
    } else {
        return false;
    }
}

bool SAMV71_UART::tx(uint8_t *p_data) {
    return m_tx_buffer.get(p_data);
}

static void isr_error(void *p_user, uint32_t diag) {
    if (p_user) {
        ((SAMV71_UART *) p_user)->error(diag);
    }
}

void SAMV71_UART::error(uint32_t diag) {
    // TODO: The very first read will cause an OVRE
//    while (true);
}

void SAMV71_UART::setup_interrupts() {
    isr_serial_callbacks callbacks = {m_definition.irq_number, isr_rx, isr_tx, isr_error, (void *) this};
    register_USART_interrupt_handler(callbacks);
}

void SAMV71_UART::reset() const {
    m_definition.p_USART->US_WPMR = US_WPMR_WPKEY_PASSWD;
    m_definition.p_USART->US_MR = 0;
    m_definition.p_USART->US_RTOR = 0;
    m_definition.p_USART->US_TTGR = 0;
    m_definition.p_USART->US_CR = US_CR_RSTRX | US_CR_RXDIS;
    m_definition.p_USART->US_CR = US_CR_RSTTX | US_CR_TXDIS;
    m_definition.p_USART->US_CR = US_CR_RSTSTA;
    m_definition.p_USART->US_CR = US_CR_RTSDIS;
}

uint32_t SAMV71_UART::setup_UART(uint32_t bitrate) {
    Clock_interface::enable_peripheral_clock(m_definition.USART_number);
    rx_pin.initialize();
    tx_pin.initialize();
    reset();
    if (set_bitrate(bitrate)) {
        return 1;
    }
    m_definition.p_USART->US_MR |=
            US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_CHMODE_NORMAL | US_MR_NBSTOP_1_BIT | US_MR_USART_MODE_NORMAL;
    m_definition.p_USART->US_CR = US_CR_RXEN;
    m_definition.p_USART->US_CR = US_CR_TXEN;
    return 0;
}

uint32_t SAMV71_UART::set_bitrate(uint32_t bitrate) const {
    uint32_t peripheral_clock = Clock_interface::get_peripheral_hz();
    uint32_t over;
    uint32_t cd_fp;
    uint32_t cd;
    uint32_t fp;
    /* Calculate the receiver sampling divide of baudrate clock. */
    if (peripheral_clock >= HIGH_FRQ_SAMPLE_DIV * bitrate) {
        over = HIGH_FRQ_SAMPLE_DIV;
    } else {
        over = LOW_FRQ_SAMPLE_DIV;
    }
    /* Calculate clock divider according to the fraction calculated formula. */
    cd_fp = (8 * peripheral_clock + (over * bitrate) / 2) / (over * bitrate);
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

bool SAMV71_UART::is_ready() {
    return (m_rx_buffer.is_empty() && m_tx_buffer.is_empty());
}
