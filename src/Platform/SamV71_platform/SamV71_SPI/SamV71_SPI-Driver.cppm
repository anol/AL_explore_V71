//
// Created by aeols on 2026-11-04.
//

module;

#include <cstdio>

#include "samv71q21b.h"

module Platform.SamV71_SPI:Driver;
import :Define;

import Platform.FreeRTOS_queue;
import Type.Abstract_request;
import Platform.SamV71_clock;

namespace SamV71 {
    class SamV71_SPI_driver {
        enum {
            Queue_size             = 64,
            SPI_CR_clock_polarity  = SPI_CSR_CPOL(0),
            SPI_CR_clock_phase     = SPI_CSR_NCPHA(1),
            SPI_MR_host_mode       = SPI_MR_MSTR(1),
            SPI_MR_fault_detection = SPI_MR_MODFDIS(1),
            SPI_bitrate            = 2'000'000,
        };

        const SamV71_SPI_define *optional_define;
        FreeRTOS::FreeRTOS_queue<Abstract::Abstract_request *, Queue_size> the_queue{};

    public:
        explicit SamV71_SPI_driver(const SamV71_SPI_define *define) : optional_define(define) {
        }

        void print_diagnostics() const;

        void setup_SPI_registers() const;

        void enable_SPI() const;

        void disable_SPI() const;

        bool transfer(Abstract::Abstract_request *request);

        Abstract::Abstract_request *ISR_get_request(uint32_t &context_switch);
    };

    void SamV71_SPI_driver::print_diagnostics() const {
        if (optional_define) {
            printf("SPI%d: pri=%d, bps=%d, cpol=%d, cpha=%d\r\n",
                   optional_define->the_ID_number, optional_define->the_IRQ_priority, SPI_bitrate, SPI_CR_clock_polarity, SPI_CR_clock_phase);
        }
    }

    void SamV71_SPI_driver::setup_SPI_registers() const {
        constexpr auto Control_enable = SPI_CR_SPIEN(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto bitrate_divider = SPI_CSR_SCBR(SamV71_clock::get_frequency() / SPI_bitrate);
            base->SPI_CSR[0] = bitrate_divider | SPI_CR_clock_polarity | SPI_CR_clock_phase;
            base->SPI_MR = SPI_MR_host_mode | SPI_MR_fault_detection;
            base->SPI_CR |= Control_enable;
            const auto irq_number = optional_define->the_IRQ_number;
            const auto priority = optional_define->the_IRQ_priority;
            NVIC_DisableIRQ(irq_number);
            NVIC_SetPriority(irq_number, priority);
        }
    }

    void SamV71_SPI_driver::enable_SPI() const {
        constexpr auto Interrupt_enable = SPI_IER_RDRF(1) | SPI_IER_TDRE(1);
        constexpr auto Interrupt_mask = SPI_IMR_RDRF(1) | SPI_IMR_TDRE(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto irq_number = optional_define->the_IRQ_number;
            if ((base->SPI_IMR & Interrupt_mask) != Interrupt_mask) {
                NVIC_DisableIRQ(irq_number);
                base->SPI_IER = Interrupt_enable;
                NVIC_EnableIRQ(irq_number);
            }
        }
    }

    void SamV71_SPI_driver::disable_SPI() const {
        constexpr auto Interrupt_disable =
                SPI_IDR_RDRF(1) | SPI_IDR_TDRE(1) | SPI_IDR_MODF(1) | SPI_IDR_OVRES(1) |
                SPI_IDR_NSSR(1) | SPI_IDR_TXEMPTY(1) | SPI_IDR_UNDES(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto irq_number = optional_define->the_IRQ_number;
            NVIC_DisableIRQ(irq_number);
            base->SPI_IDR = Interrupt_disable;
            NVIC_ClearPendingIRQ(irq_number);
        }
    }

    bool SamV71_SPI_driver::transfer(Abstract::Abstract_request *request) {
        bool success{};
        if (request) {
            if (the_queue.send(request)) {
                enable_SPI();
                request->get_semaphore().take();
                success = true;
            }
        }
        return success;
    }

    Abstract::Abstract_request *SamV71_SPI_driver::ISR_get_request(uint32_t &context_switch) {
        Abstract::Abstract_request *request;
        if (the_queue.ISR_receive(&request, context_switch)) {
            return request;
        }
        return {};
    }
}
