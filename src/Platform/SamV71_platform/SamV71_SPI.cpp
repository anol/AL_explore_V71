

module;
#include <cstdint>
#include "sam.h"
#include "samv71q21b.h"
#include <component/spi.h>

module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;
import Platform.SamV71_clock;

// struct spi_proxy_counters_t {
//     uint32_t cnt_idle;
//     uint32_t cnt_not_ready;
//     uint32_t cnt_busy_start;
//     uint32_t cnt_start;
//     uint32_t cnt_finish;
//     uint32_t cnt_fetch;
//     uint32_t cnt_store;
//     uint32_t error_rx_overrun_count;
//     uint32_t tx_no_more;
//     uint32_t rx_outside_count;
//     uint32_t already_finish;
//     uint32_t transaction_complete;
//     uint32_t total_count;
//     uint32_t rx_more;
//     uint32_t rx_done;
//     uint32_t tx_more;
//     uint32_t tx_done;
//     uint32_t tx_empty;
//     uint32_t error_overrun_count;
//     uint32_t error_underrun_count;
//     uint32_t nss_raising;
//     uint32_t error_mode_fault_count;
//     uint32_t queue_full;
// };
//
// static spi_proxy_counters_t spi_proxy_counters{};
//
// bool on_rx_ready(uint8_t data) {
//     if (is_ready()) {
//         spi_proxy_counters.rx_outside_count++;
//         return false;
//     } else {
//         uint32_t size  = the_transaction.size;
//         uint32_t count = the_transaction.rx_count;
//         if (count < size) {
//             spi_proxy_counters.cnt_store++;
//             Size_adapter::store_byte(const_cast<uint32_t *>(the_transaction.rx_buffer), the_transaction.rx_count, data);
//             the_transaction.rx_count++;
//         } else {
//             spi_proxy_counters.error_rx_overrun_count++;
//         }
//         return the_transaction.rx_count < size;
//     }
// }
//
// bool on_tx_ready(uint8_t *p_data) {
//     bool valid_data = false;
//     if (is_ready()) {
//         *p_data = 0u;
//     } else {
//         uint32_t size = the_transaction.size;
//         if (the_transaction.tx_count < size) {
//             spi_proxy_counters.cnt_fetch++;
//             *p_data = Size_adapter::fetch_byte(const_cast<const uint32_t *>(the_transaction.tx_buffer),
//                                                the_transaction.tx_count, the_transaction.write_flag);
//             the_transaction.tx_count++;
//             valid_data = true;
//         } else {
//             spi_proxy_counters.tx_no_more++;
//             *p_data = 0u;
//         }
//     }
//     return valid_data;
// }
//
// void execute_pending_transaction() {
//     if (is_ready()) {
//         SPI_transaction transaction{};
//         if (the_request_queue.get(&transaction)) {
//             set_busy();
//             spi_proxy_counters.cnt_start++;
//             the_transaction          = transaction;
//             the_transaction.tx_count = 0;
//             the_transaction.rx_count = 0;
//             the_before_timestamp     = Clock_interface::get_microsecond_timestamp();
//             CS0_pin.clear();
//             enable_SPI();
//         } else {
//             spi_proxy_counters.cnt_idle++;
//         }
//     } else spi_proxy_counters.cnt_not_ready++;
// }
//
// bool is_transaction_complete() {
//     if (is_ready()) {
//         spi_proxy_counters.already_finish++;
//         CS0_pin.set();
//         disable_SPI();
//         execute_pending_transaction();
//         return true;
//     } else {
//         uint32_t size = the_transaction.size;
//         if ((the_transaction.tx_count >= size) && (the_transaction.rx_count >= size)) {
//             spi_proxy_counters.cnt_finish++;
//             CS0_pin.set();
//             the_after_timestamp = Clock_interface::get_microsecond_timestamp();
//             disable_SPI();
//             end_transaction(true);
//             set_ready();
//             execute_pending_transaction();
//             return true;
//         } else {
//             return false;
//         }
//     }
// }
//
// void SPI0_handler() {
//     auto *   base           = SPI0_REGS;
//     uint32_t interrupt_mask = base->SPI_IMR;
//     spi_proxy_counters.total_count++;
//     if ((interrupt_mask & SPI_IMR_TDRE_Msk) && (base->SPI_SR & SPI_SR_TDRE_Msk)) {
//         uint8_t data;
//         if (on_tx_ready(&data)) {
//             spi_proxy_counters.tx_more++;
//             base->SPI_TDR = SPI_TDR_TD(data);
//         } else {
//             spi_proxy_counters.tx_done++;
//             base->SPI_IDR = SPI_IDR_TDRE(1);
//         }
//     }
//     if ((interrupt_mask & SPI_IMR_RDRF_Msk) && (base->SPI_SR & SPI_SR_RDRF_Msk)) {
//         uint8_t data = SPI_RDR_RD_Msk & base->SPI_RDR;
//         if (on_rx_ready(data)) {
//             spi_proxy_counters.rx_more++;
//         } else {
//             spi_proxy_counters.rx_done++;
//             base->SPI_IDR = SPI_IDR_RDRF(1);
//         }
//     }
//     if (is_transaction_complete()) {
//         spi_proxy_counters.transaction_complete++;
//     }
// }

extern "C" {
void SPI0_Handler()
{
    NVIC_DisableIRQ(SPI0_IRQn);
}
}

namespace SamV71
{
    void SamV71_SPI::initialize()
    {
        SamV71_clock::enable_peripheral_clock(SPI0_INSTANCE_ID);
        disable_SPI();
        setup_SPI_registers(SamV71_clock::get_frequency() / SPI_bitrate);
        enable_SPI();
    }

    void SamV71_SPI::setup_SPI_registers(uint32_t bitrate_divider) const
    {
        auto base = SPI0_REGS;
        base->SPI_CSR[0] =
            SPI_CSR_SCBR(bitrate_divider) |
            SPI_CSR_NCPHA(1) | // Clock phase
            SPI_CSR_DLYBS(0u) | // Delay before select
            SPI_CSR_DLYBCT(0u); // Delay between consecutive transfers
        base->SPI_MR = SPI_MR_MSTR(1);
    }

    void SamV71_SPI::enable_SPI() const
    {
        auto base = SPI0_REGS;
        base->SPI_CR |= SPI_CR_SPIEN(1);
        NVIC_DisableIRQ(SPI0_IRQn);
        base->SPI_IER = SPI_IER_RDRF(1) | SPI_IER_TDRE(1);
        NVIC_ClearPendingIRQ(SPI0_IRQn);
        NVIC_EnableIRQ(SPI0_IRQn);
    }

    void SamV71_SPI::disable_SPI() const
    {
        auto base = SPI0_REGS;
        NVIC_DisableIRQ(SPI0_IRQn);
        base->SPI_IDR = (
            SPI_IDR_RDRF(1) |
            SPI_IDR_TDRE(1) |
            SPI_IDR_MODF(1) |
            SPI_IDR_OVRES(1) |
            SPI_IDR_NSSR(1) |
            SPI_IDR_TXEMPTY(1) |
            SPI_IDR_UNDES(1));
        NVIC_ClearPendingIRQ(SPI0_IRQn);
        NVIC_EnableIRQ(SPI0_IRQn);
    }

    bool SamV71_SPI::transfer(Abstract::Abstract_request* request)
    {
        bool success{};
        auto* semaphore = request->get_semaphore();
        if (request && semaphore)
        {
            if (the_queue.send(request))
            {
                semaphore->take();
                success = true;
            }
        }
        return success;
    }
} // SamV71
