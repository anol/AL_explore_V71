/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains
 * the property of Integrated Detector Electronics AS and its suppliers,
 * if any. The intellectual and technical concepts contained
 * herein are proprietary to Integrated Detector Electronics AS
 * and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Integrated Detector Electronics AS.
 */
/**
 * \date   IDEAS/23.08.2019/aeols
 * \brief
 */

#include <cstdint>
#include <stdio.h>

#include <Size_adapter.h>
#include <samv71q21b_symbols.h>
#include <component/pmc/pmc_driver.h>
#include <component/pmc.h>
#include <component/clock/sysclk.h>
#include <component/spi/SPI_interrupt.h>
#include <cortex_m7_definitions.h>
#include <nvic_helpers.h>

#include "component/clock/SAMV71_clock.h"
#include "SAMV71_SPI.h"
#include "Common/NORM_interface_definition.h"

using namespace NORM_interface;

struct spi_proxy_counters_t {
    uint32_t cnt_idle;
    uint32_t cnt_not_ready;
    uint32_t cnt_busy_start;
    uint32_t cnt_start;
    uint32_t cnt_finish;
    uint32_t cnt_fetch;
    uint32_t cnt_store;
    uint32_t error_rx_overrun_count;
    uint32_t tx_no_more;
    uint32_t rx_outside_count;
    uint32_t already_finish;
    uint32_t transaction_complete;
    uint32_t total_count;
    uint32_t rx_more;
    uint32_t rx_done;
    uint32_t tx_more;
    uint32_t tx_done;
    uint32_t tx_empty;
    uint32_t error_overrun_count;
    uint32_t error_underrun_count;
    uint32_t nss_raising;
    uint32_t error_mode_fault_count;
    uint32_t queue_full;
};

static spi_proxy_counters_t spi_proxy_counters{};

SAMV71_SPI::SAMV71_SPI(const SPI_definition &definition) :
        the_definition(definition),
        MISO_pin(definition.miso_port, definition.miso_pin, false, definition.miso_mode),
        MOSI_pin(definition.mosi_port, definition.mosi_pin, true, definition.mosi_mode),
        Clock_pin(definition.clock_port, definition.clock_pin, true, definition.clock_mode),
        CS0_pin(definition.nss_port, definition.nss_pin, true, SAMV71_GPIO::GPIO_MODE) {}

static void inner_callback(void *user) {
    if (user != nullptr) (static_cast<SAMV71_SPI *>(user))->on_interrupt();
}

void SAMV71_SPI::initialize(uint32_t bitrate) {
    uint32_t clock_frequency = Clock_interface::enable_peripheral_clock(the_definition.peripheral_id);
    MOSI_pin.initialize();
    MISO_pin.initialize();
    Clock_pin.initialize();
    CS0_pin.initialize();
    CS0_pin.set();
    CS0_pin.enable();
    the_transaction.optional_user = nullptr;
    the_transaction.size = 0;
    the_transaction.tx_count = 0;
    the_transaction.rx_count = 0;
    disable_SPI();
    setup_SPI_registers(clock_frequency / bitrate);
    SPI_interrupt::register_handler(the_definition.SPI_number,
                                    {the_definition.peripheral_id, (void *) this, inner_callback});
    set_ready();
    enable_SPI();
}

void SAMV71_SPI::reset() {
    disable_SPI();
    the_transaction.optional_user = nullptr;
    the_transaction.size = 0;
    the_transaction.tx_count = 0;
    the_transaction.rx_count = 0;
    set_ready();
    enable_SPI();
}

void SAMV71_SPI::setup_SPI_registers(uint32_t bitrate_divider) const {
    auto base = the_definition.p_SPI;
    base->SPI_CSR[0] =
            SPI_CSR_SCBR(bitrate_divider) |
            SPI_CSR_NCPHA | // Clock phase
            SPI_CSR_DLYBS(0u) | // Delay before select
            SPI_CSR_DLYBCT(0u); // Delay between consecutive transfers
    base->SPI_MR = SPI_MR_MSTR;
}

void SAMV71_SPI::enable_SPI() const {
    auto base = the_definition.p_SPI;
    base->SPI_CR |= SPI_CR_SPIEN;
    NVIC_DisableIRQ(the_definition.peripheral_id);
    base->SPI_IER = SPI_IER_RDRF | SPI_IER_TDRE;
    NVIC_ClearPendingIRQ(the_definition.peripheral_id);
    NVIC_EnableIRQ(the_definition.peripheral_id);
}

void SAMV71_SPI::disable_SPI() const {
    auto base = the_definition.p_SPI;
    NVIC_DisableIRQ(the_definition.peripheral_id);
    base->SPI_IDR = (
            SPI_IDR_RDRF |
            SPI_IDR_TDRE |
            SPI_IDR_MODF |
            SPI_IDR_OVRES |
            SPI_IDR_NSSR |
            SPI_IDR_TXEMPTY |
            SPI_IDR_UNDES);
    NVIC_ClearPendingIRQ(the_definition.peripheral_id);
    NVIC_EnableIRQ(the_definition.peripheral_id);
}

bool SAMV71_SPI::start_transaction(const SPI_transaction &transaction) {
    bool success = the_request_queue.put(transaction);
    if (!success) spi_proxy_counters.queue_full++;
    execute_pending_transaction();
    return success;
}

void SAMV71_SPI::end_transaction(bool success) {
    void *p_user = the_transaction.optional_user;
    SPI_interface::SPI_finish_t p_function = the_transaction.optional_func;
    the_transaction.clear();
    p_function(p_user, success, the_before_timestamp, the_after_timestamp);
}

void SAMV71_SPI::execute_pending_transaction() {
    if (is_ready()) {
        SPI_transaction transaction{};
        if (the_request_queue.get(&transaction)) {
            set_busy();
            spi_proxy_counters.cnt_start++;
            the_transaction = transaction;
            the_transaction.tx_count = 0;
            the_transaction.rx_count = 0;
            the_before_timestamp = Clock_interface::get_microsecond_timestamp();
            CS0_pin.clear();
            enable_SPI();
        } else {
            spi_proxy_counters.cnt_idle++;
        }
    } else spi_proxy_counters.cnt_not_ready++;
}

void SAMV71_SPI::on_interrupt() {
    auto *base = the_definition.p_SPI;
    uint32_t interrupt_mask = base->SPI_IMR;
    spi_proxy_counters.total_count++;
    if ((interrupt_mask & SPI_IMR_TDRE) && (base->SPI_SR & SPI_SR_TDRE)) {
        uint8_t data;
        if (on_tx_ready(&data)) {
            spi_proxy_counters.tx_more++;
            base->SPI_TDR = SPI_TDR_TD(data);
        } else {
            spi_proxy_counters.tx_done++;
            base->SPI_IDR = (SPI_IDR_TDRE);
        }
    }
    if ((interrupt_mask & SPI_IMR_RDRF) && (base->SPI_SR & SPI_SR_RDRF)) {
        uint8_t data = SPI_RDR_RD_Msk & base->SPI_RDR;
        if (on_rx_ready(data)) {
            spi_proxy_counters.rx_more++;
        } else {
            spi_proxy_counters.rx_done++;
            base->SPI_IDR = (SPI_IDR_RDRF);
        }
    }
    if (is_transaction_complete()) {
        spi_proxy_counters.transaction_complete++;
    }
}

bool SAMV71_SPI::on_rx_ready(uint8_t data) {
    if (is_ready()) {
        spi_proxy_counters.rx_outside_count++;
        return false;
    } else {
        uint32_t size = the_transaction.size;
        uint32_t count = the_transaction.rx_count;
        if (count < size) {
            spi_proxy_counters.cnt_store++;
            Size_adapter::store_byte(const_cast<uint32_t *>(the_transaction.rx_buffer), the_transaction.rx_count, data);
            the_transaction.rx_count++;
        } else {
            spi_proxy_counters.error_rx_overrun_count++;
        }
        return the_transaction.rx_count < size;
    }
}

bool SAMV71_SPI::on_tx_ready(uint8_t *p_data) {
    bool valid_data = false;
    if (is_ready()) {
        *p_data = 0u;
    } else {
        uint32_t size = the_transaction.size;
        if (the_transaction.tx_count < size) {
            spi_proxy_counters.cnt_fetch++;
            *p_data = Size_adapter::fetch_byte(const_cast<const uint32_t *>(the_transaction.tx_buffer),
                                               the_transaction.tx_count, the_transaction.write_flag);
            the_transaction.tx_count++;
            valid_data = true;
        } else {
            spi_proxy_counters.tx_no_more++;
            *p_data = 0u;
        }
    }
    return valid_data;
}

bool SAMV71_SPI::is_transaction_complete() {
    if (is_ready()) {
        spi_proxy_counters.already_finish++;
        CS0_pin.set();
        disable_SPI();
        execute_pending_transaction();
        return true;
    } else {
        uint32_t size = the_transaction.size;
        if ((the_transaction.tx_count >= size) && (the_transaction.rx_count >= size)) {
            spi_proxy_counters.cnt_finish++;
            CS0_pin.set();
            the_after_timestamp = Clock_interface::get_microsecond_timestamp();
            disable_SPI();
            end_transaction(true);
            set_ready();
            execute_pending_transaction();
            return true;
        } else {
            return false;
        }
    }
}

void SAMV71_SPI::report(Service_report &reporter) {
    reporter.begin_object(NORM_interface::Key_spi);
    reporter.report(Key_ready, is_ready());
    reporter.report(Key_count, the_request_queue.get_put_count());
    reporter.report(Key_overflow, the_request_queue.get_overflow_count());
    reporter.report(Key_queue_fill, the_request_queue.get_fill());
    reporter.report(Key_queue_max, the_request_queue.get_max_fill());
    reporter.report(Key_queue_size, the_request_queue.get_size());
    reporter.report(Key_cnt_idle, spi_proxy_counters.cnt_idle);
    reporter.report(Key_cnt_tx, the_transaction.tx_count);
    reporter.report(Key_cnt_rx, the_transaction.rx_count);
    reporter.end_object();

    // TODO: SPI diagnostic counters
//    reporter.report("SPI_is_idle", is_idle());
//    reporter.report("SPI_pending_requests", the_request_queue.get_fill());
//    reporter.report("SPI_transaction", is_ready());
//    reporter.report("SPI_transaction_size", the_transaction.size);
//    reporter.report("SPI_TX_count", the_transaction.tx_count);
//    reporter.report("SPI_RX_count", the_transaction.rx_count);
//    reporter.report("SPI_mode_register", use_definition.p_SPI->SPI_MR);
//    reporter.report("SPI_status_register", use_definition.p_SPI->SPI_SR);
//    // Normally '0'
//    reporter.report("SPI_interrupt_mask", use_definition.p_SPI->SPI_IMR);
//    reporter.report("SPI_error_mode_fault_count", spi_proxy_counters.error_mode_fault_count);
//    reporter.report("SPI_error_overrun", spi_proxy_counters.error_overrun_count);
//    reporter.report("SPI_error_underrun", spi_proxy_counters.error_underrun_count);
//    reporter.report("SPI_count_nss_raising", spi_proxy_counters.nss_raising);
//    reporter.report("SPI_error_rx_overrun", spi_proxy_counters.error_rx_overrun_count);
//    reporter.report("SPI_count_rx_outside", spi_proxy_counters.rx_outside_count);
//    reporter.report("SPI_count_tx_empty", spi_proxy_counters.tx_empty);
//    reporter.report("SPI_count_busy_start", spi_proxy_counters.cnt_busy_start);
//    // Number of transactions
//    reporter.report("SPI_count_start", spi_proxy_counters.cnt_start);
//    reporter.report("SPI_count_tx_no_more", spi_proxy_counters.tx_no_more);
//    reporter.report("SPI_count_rx_done", spi_proxy_counters.rx_done);
//    reporter.report("SPI_count_finish", spi_proxy_counters.cnt_finish);
//    reporter.report("SPI_count_allready_finish", spi_proxy_counters.already_finish);
//    reporter.report("SPI_count_tx_done", spi_proxy_counters.tx_done);
//    reporter.report("SPI_count_trans_complete", spi_proxy_counters.transaction_complete);
//    reporter.report("SPI_count_idle", spi_proxy_counters.cnt_idle);
//    reporter.report("SPI_count_not_ready", spi_proxy_counters.cnt_not_ready);
//    // Number of bytes
//    reporter.report("SPI_count_fetch", spi_proxy_counters.cnt_fetch);
//    reporter.report("SPI_count_store", spi_proxy_counters.cnt_store);
//    reporter.report("SPI_count_tx_more", spi_proxy_counters.tx_more);
//    reporter.report("SPI_count_rx_more", spi_proxy_counters.rx_more);
//    reporter.report("SPI_count_total", spi_proxy_counters.total_count);
}
