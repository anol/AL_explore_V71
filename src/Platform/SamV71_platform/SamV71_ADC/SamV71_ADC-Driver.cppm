//
// Created by aeols on 2026-11-11.
//

module;

#include <cstdio>

#include "samv71q21b.h"
#include "component/afec.h"

module Platform.SamV71_ADC:Driver;
import :Define;

import Platform.FreeRTOS_queue;
import Platform.FreeRTOS_semaphore;
import Type.Abstract_request;
import Platform.SamV71_clock;

namespace SamV71 {
     class SamV71_ADC_request : public Abstract::Abstract_request {
        mutable FreeRTOS::FreeRTOS_semaphore the_semaphore{};
        const uint8_t the_channel;
        uint32_t the_value{};
        bool the_completed{};

    public:
        explicit SamV71_ADC_request(const uint8_t channel) : the_channel(channel) {
        }

        [[nodiscard]] Abstract::Abstract_semaphore &get_semaphore() const override { return the_semaphore; }
        [[nodiscard]] uint8_t get_channel() const { return the_channel; }
        [[nodiscard]] uint32_t get_value() const { return the_value; }
        [[nodiscard]] bool is_completed() const { return the_completed; }

        void ISR_complete(const uint32_t value) {
            the_value = value;
            the_completed = true;
        }
    };

    class SamV71_ADC_driver {
        enum : uint32_t {
            Queue_size      = 8,
            AFE_clock       = 6'000'000, // fAFE = MCK / (PRESCAL + 1)
            Tracking_time   = 2,
            Transfer_period = 1,
            Bias_current    = 1,
            Analog_offset   = 0x200,     // Mid-scale offset for single-ended input
        };

        const SamV71_ADC_define *optional_define;
        FreeRTOS::FreeRTOS_queue<SamV71_ADC_request *, Queue_size> the_queue{};

    public:
        explicit SamV71_ADC_driver(const SamV71_ADC_define *define) : optional_define(define) {
        }

        static constexpr uint32_t channel_mask(const uint8_t channel) { return 1u << channel; }

        void print_diagnostics() const;

        void enable_clock() const;

        void setup_ADC_registers() const;

        void configure_channel(uint8_t channel) const;

        void enable_ADC() const;

        void disable_ADC() const;

        bool convert(SamV71_ADC_request *request);

        SamV71_ADC_request *ISR_get_request(uint32_t &context_switch);
    };

    void SamV71_ADC_driver::print_diagnostics() const {
        if (optional_define) {
            const auto *base = optional_define->the_base;
            printf("AFEC%d: pri=%d, fAFE=%d, CHSR=0x%03x, IMR=0x%08x\r\n",
                   optional_define->the_ID_number, static_cast<int>(optional_define->the_IRQ_priority),
                   static_cast<int>(AFE_clock), static_cast<unsigned>(base->AFEC_CHSR), static_cast<unsigned>(base->AFEC_IMR));
        }
    }

    void SamV71_ADC_driver::enable_clock() const {
        if (optional_define) {
            SamV71_clock::enable_peripheral_clock(optional_define->the_peripheral_id);
        }
    }

    void SamV71_ADC_driver::setup_ADC_registers() const {
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto prescaler = SamV71_clock::get_frequency() / AFE_clock - 1;
            base->AFEC_CR = AFEC_CR_SWRST_Msk;
            base->AFEC_MR = AFEC_MR_TRGEN_DIS |          // Software trigger only
                            AFEC_MR_PRESCAL(prescaler) |
                            AFEC_MR_STARTUP_SUT64 |
                            AFEC_MR_ONE_Msk |            // Must be written to 1
                            AFEC_MR_TRACKTIM(Tracking_time) |
                            AFEC_MR_TRANSFER(Transfer_period);
            base->AFEC_EMR = AFEC_EMR_RES_NO_AVERAGE |   // 12-bit, no averaging
                             AFEC_EMR_TAG_Msk |
                             AFEC_EMR_STM_Msk;
            base->AFEC_ACR = AFEC_ACR_IBCTL(Bias_current) | AFEC_ACR_PGA0EN_Msk | AFEC_ACR_PGA1EN_Msk;
            base->AFEC_TEMPMR = 0;                       // Temperature sensor converts on software trigger, not RTC
            base->AFEC_CHDR = AFEC_CHDR_Msk;             // configure_channel() enables the used channel
            base->AFEC_IDR = AFEC_IDR_Msk;
            const auto irq_number = optional_define->the_IRQ_number;
            const auto priority = optional_define->the_IRQ_priority;
            NVIC_DisableIRQ(irq_number);
            NVIC_SetPriority(irq_number, priority);
        }
    }

    void SamV71_ADC_driver::configure_channel(const uint8_t channel) const {
        if (optional_define) {
            auto *base = optional_define->the_base;
            base->AFEC_DIFFR &= ~channel_mask(channel);       // Single-ended
            base->AFEC_CGR &= ~(0x03u << (2 * channel));      // Gain = 1
            base->AFEC_CSELR = channel;
            base->AFEC_COCR = Analog_offset & AFEC_COCR_AOFF_Msk;
            // Keep the channel enabled: enabling it hands the PIO pin over to the analog input,
            // so toggling it per conversion would leave too little settling time.
            base->AFEC_CHER = channel_mask(channel);
        }
    }

    void SamV71_ADC_driver::enable_ADC() const {
        if (optional_define) {
            const auto irq_number = optional_define->the_IRQ_number;
            // Kick the ISR; it picks up the queued request if the AFEC is idle,
            // and ignores the kick if a conversion is already in flight.
            NVIC_EnableIRQ(irq_number);
            NVIC_SetPendingIRQ(irq_number);
        }
    }

    void SamV71_ADC_driver::disable_ADC() const {
        if (optional_define) {
            auto *base = optional_define->the_base;
            const auto irq_number = optional_define->the_IRQ_number;
            NVIC_DisableIRQ(irq_number);
            base->AFEC_IDR = AFEC_IDR_Msk;
            NVIC_ClearPendingIRQ(irq_number);
        }
    }

    bool SamV71_ADC_driver::convert(SamV71_ADC_request *request) {
        bool success{};
        if (request && optional_define) {
            if (the_queue.send(request)) {
                enable_ADC();
                request->get_semaphore().take();
                success = request->is_completed();
            }
        }
        return success;
    }

    SamV71_ADC_request *SamV71_ADC_driver::ISR_get_request(uint32_t &context_switch) {
        SamV71_ADC_request *request;
        if (the_queue.ISR_receive(&request, context_switch)) {
            return request;
        }
        return {};
    }
}
