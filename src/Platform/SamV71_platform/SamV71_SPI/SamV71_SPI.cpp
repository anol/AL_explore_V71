module;

#include <cstdint>
#include <cstdio>

#include "sam.h"
#include "samv71q21b.h"
#include "component/spi.h"

#include "FreeRTOS.h"

module Platform.SamV71_SPI;
import Type.Abstract_SPI;
import Type.Transfer_request;
import Platform.FreeRTOS_queue;
import Platform.SamV71_clock;
import Platform.SamV71_IO_pin;

namespace SamV71 {
    SamV71_SPI::SamV71_SPI(const uint8_t id, Abstract::Abstract_IO_pin &chip_select)
        : the_id(id), use_chip_select(chip_select), optional_define(SamV71_SPI_define::get_definition(id)) {
        configASSERT(optional_define != nullptr);
    }

    void SamV71_SPI::print_diagnostics() const {
        if (optional_define) {
            const auto priority = optional_define->the_IRQ_priority;
            printf("SPI%d: pri=%d, bps=%d, cpol=%d, cpha=%d\r\n", the_id, priority, SPI_bitrate, SPI_CR_clock_polarity, SPI_CR_clock_phase);
        }
    }

    void SamV71_SPI::initialize() {
        SamV71_clock::enable_peripheral_clock(SPI0_INSTANCE_ID);
        disable_SPI();
        setup_SPI_registers();
        enable_SPI();
    }

    void SamV71_SPI::setup_SPI_registers() const {
        constexpr auto Control_enable = SPI_CR_SPIEN(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto bitrate_divider = SPI_CSR_SCBR(SamV71_clock::get_frequency() / SPI_bitrate);
            base->SPI_CSR[0] = bitrate_divider | SPI_CR_clock_polarity | SPI_CR_clock_phase;
            base->SPI_MR = SPI_MR_host_mode | SPI_MR_fault_detection;
            base->SPI_CR |= Control_enable;
        }
    }

    void SamV71_SPI::enable_SPI() const {
        constexpr auto Interrupt_enable = SPI_IER_RDRF(1) | SPI_IER_TDRE(1);
        constexpr auto Interrupt_mask = SPI_IMR_RDRF(1) | SPI_IMR_TDRE(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto irq_number = optional_define->the_IRQ_number;
            const auto priority = optional_define->the_IRQ_priority;
            if ((base->SPI_IMR & Interrupt_mask) == 0) {
                NVIC_DisableIRQ(irq_number);
                NVIC_SetPriority(irq_number, priority);
                base->SPI_IER = Interrupt_enable;
                NVIC_ClearPendingIRQ(irq_number);
                NVIC_EnableIRQ(irq_number);
            }
        }
    }

    void SamV71_SPI::disable_SPI() const {
        constexpr auto Interrupt_disable =
                SPI_IDR_RDRF(1) | SPI_IDR_TDRE(1) | SPI_IDR_MODF(1) | SPI_IDR_OVRES(1) |
                SPI_IDR_NSSR(1) | SPI_IDR_TXEMPTY(1) | SPI_IDR_UNDES(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto irq_number = optional_define->the_IRQ_number;
            NVIC_DisableIRQ(irq_number);
            base->SPI_IDR = Interrupt_disable;
            NVIC_ClearPendingIRQ(irq_number);
            NVIC_EnableIRQ(irq_number);
        }
    }

    bool SamV71_SPI::transfer(Abstract::Abstract_request *request) {
        bool success{};
        if (request) {
            if (the_queue.send(request)) {
                enable_SPI(); // pending_transaction();
                request->get_semaphore().take();
                success = true;
            }
        }
        return success;
    }
} // SamV71
