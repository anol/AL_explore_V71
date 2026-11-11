//
// Created by aeols on 2026-11-11.
//

module;

#include "FreeRTOS.h"
#include "portmacro.h"
#include "samv71q21b.h"
#include "component/afec.h"

module Platform.SamV71_ADC:Handler;
import :Define;
import :Driver;

namespace SamV71 {
    class SamV71_ADC_handler {
    public:
        enum { Number_of_ADC_peripherals = 1 };

        static SamV71_ADC_handler *optional_handler[Number_of_ADC_peripherals];

    private:
        const SamV71_ADC_define *optional_define;
        SamV71_ADC_driver &use_driver;
        SamV71_ADC_request *optional_request{};

    public:
        explicit SamV71_ADC_handler(const SamV71_ADC_define *define, SamV71_ADC_driver &driver)
            : optional_define(define), use_driver(driver) {
            if (define && define->the_ID_number < Number_of_ADC_peripherals) {
                optional_handler[define->the_ID_number] = this;
            }
        }

        void ISR();

    private:
        [[nodiscard]] bool is_busy() const { return optional_request != nullptr; }

        [[nodiscard]] bool is_ready() const { return optional_request == nullptr; }

        void ISR_end_of_conversion(afec_registers_t *base, uint32_t &context_switch);

        void ISR_pending_conversion(uint32_t &context_switch);
    };

    SamV71_ADC_handler *SamV71_ADC_handler::optional_handler[Number_of_ADC_peripherals]{};

    void SamV71_ADC_handler::ISR() {
        if (optional_define) {
            uint32_t context_switch{};
            auto *base = optional_define->the_base;
            const uint32_t interrupt_status = base->AFEC_ISR;
            if (is_busy() && (interrupt_status & SamV71_ADC_driver::channel_mask(optional_request->get_channel()))) {
                ISR_end_of_conversion(base, context_switch);
            }
            if (is_ready()) {
                ISR_pending_conversion(context_switch);
            }
            portYIELD_FROM_ISR(context_switch);
        }
    }

    void SamV71_ADC_handler::ISR_end_of_conversion(afec_registers_t *base, uint32_t &context_switch) {
        auto *request = optional_request;
        const auto channel = request->get_channel();
        const auto mask = SamV71_ADC_driver::channel_mask(channel);
        base->AFEC_CSELR = channel;
        const uint32_t value = base->AFEC_CDR & AFEC_CDR_DATA_Msk; // Reading CDR clears EOCx
        base->AFEC_IDR = mask;
        optional_request = nullptr;
        request->ISR_complete(value);
        request->get_semaphore().ISR_give(context_switch); // Request must not be touched after this
    }

    void SamV71_ADC_handler::ISR_pending_conversion(uint32_t &context_switch) {
        auto *base = optional_define->the_base;
        if (auto *request = use_driver.ISR_get_request(context_switch)) {
            const auto channel = request->get_channel();
            const auto mask = SamV71_ADC_driver::channel_mask(channel);
            base->AFEC_CSELR = channel;
            (void)base->AFEC_CDR; // Flush any stale result (clears EOCx)
            optional_request = request;
            base->AFEC_IER = mask; // Channel already enabled by configure_channel()
            base->AFEC_CR = AFEC_CR_START_Msk;
            return;
        }
        use_driver.disable_ADC();
    }
} // SamV71

extern "C" {
void ISR_AFEC0() {
    if (auto *handler = SamV71::SamV71_ADC_handler::optional_handler[0]) {
        handler->ISR();
    } else {
        NVIC_DisableIRQ(AFEC0_IRQn);
        NVIC_ClearPendingIRQ(AFEC0_IRQn);
    }
}
}
