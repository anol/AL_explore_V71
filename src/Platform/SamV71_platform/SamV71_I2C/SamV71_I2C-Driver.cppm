//
// Created by aeols on 2026-11-19.
//

module;

#include <cstdio>

#include "samv71q21b.h"
#include "component/twihs.h"

module Platform.SamV71_I2C:Driver;
import :Define;

import Platform.FreeRTOS_queue;
import Type.Abstract_request;
import Platform.SamV71_clock;

namespace SamV71 {
    class SamV71_I2C_driver {
        enum {
            Queue_size      = 8,
            I2C_bitrate     = 100'000,
            Max_clock_div   = 255,
            Max_clock_scale = 7,
        };

        const SamV71_I2C_define *optional_define;
        FreeRTOS::FreeRTOS_queue<Abstract::Abstract_request *, Queue_size> the_queue{};

    public:
        explicit SamV71_I2C_driver(const SamV71_I2C_define *define) : optional_define(define) {
        }

        void print_diagnostics() const;

        void setup_I2C_registers() const;

        void enable_I2C() const;

        void disable_I2C() const;

        bool transfer(Abstract::Abstract_request *request);

        Abstract::Abstract_request *ISR_get_request(uint32_t &context_switch);

    private:
        static uint32_t get_clock_waveform();
    };

    void SamV71_I2C_driver::print_diagnostics() const {
        if (optional_define) {
            const auto waveform = get_clock_waveform();
            printf("I2C%d: pri=%d, bps=%d, cldiv=%d, ckdiv=%d\r\n",
                   optional_define->the_ID_number, optional_define->the_IRQ_priority, I2C_bitrate,
                   static_cast<int>((waveform & TWIHS_CWGR_CLDIV_Msk) >> TWIHS_CWGR_CLDIV_Pos),
                   static_cast<int>((waveform & TWIHS_CWGR_CKDIV_Msk) >> TWIHS_CWGR_CKDIV_Pos));
        }
    }

    /*
     * SCL low/high time = ((xLDIV * 2^CKDIV) + 3) * T_peripheral; same value for both halves.
     */
    uint32_t SamV71_I2C_driver::get_clock_waveform() {
        uint32_t divider = SamV71_clock::get_frequency() / (2 * I2C_bitrate);
        divider = divider > 3 ? divider - 3 : 0;
        uint32_t scale{};
        while (divider > Max_clock_div && scale < Max_clock_scale) {
            divider = (divider + 1) / 2;
            scale++;
        }
        if (divider > Max_clock_div) {
            divider = Max_clock_div;
        }
        return TWIHS_CWGR_CLDIV(divider) | TWIHS_CWGR_CHDIV(divider) | TWIHS_CWGR_CKDIV(scale);
    }

    void SamV71_I2C_driver::setup_I2C_registers() const {
        if (optional_define) {
            auto *base = optional_define->the_base;
            base->TWIHS_CR = TWIHS_CR_SWRST(1);
            (void) base->TWIHS_RHR;
            base->TWIHS_CR = TWIHS_CR_MSDIS(1) | TWIHS_CR_SVDIS(1);
            base->TWIHS_CWGR = get_clock_waveform();
            base->TWIHS_CR = TWIHS_CR_MSEN(1);
            const auto irq_number = optional_define->the_IRQ_number;
            const auto priority = optional_define->the_IRQ_priority;
            NVIC_DisableIRQ(irq_number);
            NVIC_SetPriority(irq_number, priority);
        }
    }

    void SamV71_I2C_driver::enable_I2C() const {
        // Kick only: an idle host has TXCOMP set, so the ISR fires at once and picks up the queue.
        constexpr auto Interrupt_enable = TWIHS_IER_TXCOMP(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto irq_number = optional_define->the_IRQ_number;
            // TWIHS_IER is write-1-to-set, so writing it unconditionally is safe and
            // avoids the check-then-act race with the ISR on TWIHS_IMR.
            base->TWIHS_IER = Interrupt_enable;
            NVIC_EnableIRQ(irq_number);
        }
    }

    void SamV71_I2C_driver::disable_I2C() const {
        constexpr auto Interrupt_disable =
                TWIHS_IDR_TXCOMP(1) | TWIHS_IDR_RXRDY(1) | TWIHS_IDR_TXRDY(1) | TWIHS_IDR_OVRE(1) |
                TWIHS_IDR_UNRE(1) | TWIHS_IDR_NACK(1) | TWIHS_IDR_ARBLST(1);
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto irq_number = optional_define->the_IRQ_number;
            NVIC_DisableIRQ(irq_number);
            base->TWIHS_IDR = Interrupt_disable;
            NVIC_ClearPendingIRQ(irq_number);
        }
    }

    bool SamV71_I2C_driver::transfer(Abstract::Abstract_request *request) {
        bool success{};
        if (request) {
            if (the_queue.send(request)) {
                enable_I2C();
                request->get_semaphore().take();
                success = true;
            }
        }
        return success;
    }

    Abstract::Abstract_request *SamV71_I2C_driver::ISR_get_request(uint32_t &context_switch) {
        Abstract::Abstract_request *request;
        if (the_queue.ISR_receive(&request, context_switch)) {
            return request;
        }
        return {};
    }
}
