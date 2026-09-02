//
// Created by anolsen on 23.08.2019.
//

#include <cstdint>
#include <Size_adapter.h>
#include <pmc/pmc_driver.h>
#include <component/pmc.h>
#include <clock/sysclk.h>
#include <isr_spi.h>
#include <cortex_m4_definitions.h>
#include <nvic_helpers.h>
#include <Consol.h>

#include "SPI_proxy.h"

struct spi_proxy_counters_t {
    int busy_start_count;
    int start_transaction_count;
    int finish_transaction_count;
    int fetch_count;
    int store_count;
    int isr_rx_ready_count;
    int isr_tx_ready_count;
    int isr_tx_empty_count;
    int isr_error_count;
    int rx_overrun_count;
    int rx_outside_count;
    int rx_finish_1_not_tx;
    int rx_finish_2_not_tx;
    int allready_finish_count;
    int tx_empty_not_finish_count;
    int tx_finish_not_rx;
};

static spi_proxy_counters_t spi_proxy_counters{};

static bool isr_rx_ready(void *p_user, uint8_t data) {
    spi_proxy_counters.isr_rx_ready_count++;
    if (p_user) {
        return ((SPI_proxy *) p_user)->rx_ready(data);
    } else {
        return false;
    }
}

static bool isr_tx_ready(void *p_user, uint8_t *p_data) {
    spi_proxy_counters.isr_tx_ready_count++;
    if (p_user && p_data) {
        return ((SPI_proxy *) p_user)->tx_ready(p_data);
    } else {
        return false;
    }
}

static void isr_tx_empty(void *p_user) {
    spi_proxy_counters.isr_tx_empty_count++;
}

static void isr_error(void *p_user, uint32_t diag) {
    spi_proxy_counters.isr_error_count++;
    if (p_user) {
        ((SPI_proxy *) p_user)->transaction_error(diag);
    }
}

SPI_proxy::SPI_proxy(const SPI_definition &definition) :
        m_definition(definition),
        m_transaction(),
        MISO_pin(definition.miso_mode, false, definition.miso_mode),
        MOSI_pin(definition.mosi_pin, true, definition.mosi_mode),
        Clock_pin(definition.clock_pin, true, definition.clock_mode),
        Select_pin(definition.nss_pin, true, GPIO_MODE) {}

void SPI_proxy::initialize(uint32_t bitrate) {
    Select_pin.set();
    Select_pin.enable();
    isr_spi_callbacks callbacks = {isr_rx_ready, isr_tx_ready, isr_tx_empty, isr_error, this};
    m_transaction.p_user = nullptr;
    pmc_enable_periph_clk(m_definition.SPI_number);
    disable_SPI_interrupts(m_definition.irq_number);
    set_chip_select_register(bitrate);
    m_definition.p_SPI->SPI_MR =
            SPI_MR_MSTR |
            //            SPI_MR_LLB | // Local loopback
            //            SPI_MR_WDRBT | // Wait data read before transfer
            SPI_TDR_PCS(0u);
    register_SPI_interrupt_handler(m_definition.irq_number, callbacks);
    m_definition.p_SPI->SPI_CR |= SPI_CR_SPIEN;
    enable_SPI_interrupts(m_definition.irq_number);
}

void SPI_proxy::set_chip_select_register(uint32_t bitrate) {
    m_definition.p_SPI->SPI_CSR[0] =
            SPI_CSR_SCBR(sysclk_get_peripheral_hz() / bitrate) |
            SPI_CSR_NCPHA | // Clock phase
            SPI_CSR_DLYBS(0u) | // Delay before select
            SPI_CSR_DLYBCT(0u); // Delay between consecutive transfers
}

bool SPI_proxy::start_transaction(const SPI_proxy::SPI_transaction &transaction) {
    if (m_transaction.p_user == nullptr) {
        spi_proxy_counters.start_transaction_count++;
        m_transaction = transaction;
        m_transaction.tx_count = 0;
        m_transaction.rx_count = 0;
        Select_pin.clear();
        enable_SPI_interrupts(m_definition.irq_number);
        return true;
    } else {
        spi_proxy_counters.busy_start_count++;
        return false;
    }
}

bool SPI_proxy::rx_ready(uint8_t data) {
    bool need_more_bytes = false;
    if (m_transaction.p_user == nullptr) {
        spi_proxy_counters.rx_outside_count++;
    } else if (0 == m_transaction.tx_count) {
        need_more_bytes = true;
    } else if (0 == m_transaction.rx_count) {
        m_transaction.rx_count++;
        need_more_bytes = true;
    } else {
        int count = m_transaction.rx_count;
        int size = 4 * m_transaction.size;
        if (count < size) {
            spi_proxy_counters.store_count++;
            Size_adapter::store_byte(m_transaction.rx_buffer, m_transaction.size, m_transaction.rx_count - 1, data);
            m_transaction.rx_count++;
            need_more_bytes = true;
        } else if (count == size) {
            spi_proxy_counters.store_count++;
            Size_adapter::store_byte(m_transaction.rx_buffer, m_transaction.size, m_transaction.rx_count - 1, data);
            m_transaction.rx_count++;
            need_more_bytes = true;
            if (is_transaction_complete()) {
                spi_proxy_counters.rx_overrun_count++;
            } else {
                spi_proxy_counters.rx_finish_1_not_tx++;
            }
        } else {
            if (is_transaction_complete()) {
                spi_proxy_counters.rx_overrun_count++;
            } else {
                spi_proxy_counters.rx_finish_2_not_tx++;
            }
        }
    }
    return need_more_bytes;
}

bool SPI_proxy::tx_ready(uint8_t *p_data) {
    if (m_transaction.p_user == nullptr) {
        *p_data = 0x00u;
        return false;
    } else {
        int size = 4 * m_transaction.size;
        int count = 1 + m_transaction.tx_count;
        if (count <= size) {
            spi_proxy_counters.fetch_count++;
            *p_data = Size_adapter::fetch_byte(m_transaction.tx_buffer, m_transaction.size, m_transaction.tx_count++);
            return true;
        } else {
            if (!is_transaction_complete()) {
                spi_proxy_counters.tx_finish_not_rx++;
            }
            *p_data = 0u;
            return false;
        }
    }
}

void SPI_proxy::tx_empty() {
    if (!is_transaction_complete()) {
        spi_proxy_counters.tx_empty_not_finish_count++;
    }
}

void SPI_proxy::transaction_error(uint32_t diag) {
    Consol::global_printf("SPI flags: diag=0x%08X.\r\n", diag);
}

bool SPI_proxy::is_transaction_complete() {
    if (m_transaction.p_user == nullptr) {
        spi_proxy_counters.allready_finish_count++;
        return true;
    } else {
        int size = (4 * m_transaction.size);
        if ((m_transaction.tx_count >= size) && (m_transaction.rx_count > size)) {
            Select_pin.set();
            Consol::global_printf("... finish.\r\n");
            void *p_user = m_transaction.p_user;
            spi_proxy_counters.finish_transaction_count++;
            m_transaction.p_user = nullptr;
            disable_SPI_interrupts(m_definition.irq_number);
            m_transaction.finish(p_user);
            return true;
        } else {
            return false;
        }
    }
}
