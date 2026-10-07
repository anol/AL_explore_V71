module;

#include <cstdint>

#include "FreeRTOSConfig.h"
#include "sam.h"
#include "component/dacc.h"

module Platform.SamV71_DAC;
import Type.Abstract_DAC;
import Platform.SamV71_clock;

namespace SamV71_DAC_definition {
    static SamV71::SamV71_DAC *optional_DAC{};

    namespace {
        struct Definition {
            dacc_registers_t *the_base;
            IRQn_Type the_IRQ_number;
            uint32_t the_IRQ_priority;
            uint32_t the_channel;
        };
    }

    static Definition Definition_DACC{
        .the_base         = DACC_REGS,
        .the_IRQ_number   = DACC_IRQn,
        .the_IRQ_priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY,
        .the_channel      = 0,
    };

    enum {
        DACC_WP_KEY    = (0x444143),
        Analog_control = (DACC_ACR_IBCTLCH0(0x02) | DACC_ACR_IBCTLCH1(0x02)),
    };

    static void *get_definition(const uint8_t id) { return id == 0 ? static_cast<void *>(&Definition_DACC) : nullptr; }
}

namespace SamV71_DACC_test_data {
    constexpr int16_t Sin_table[] = {
        0x0, 0x080, 0x100, 0x17f, 0x1fd, 0x278, 0x2f1, 0x367, 0x3da, 0x449,
        0x4b3, 0x519, 0x579, 0x5d4, 0x629, 0x678, 0x6c0, 0x702, 0x73c, 0x76f,
        0x79b, 0x7bf, 0x7db, 0x7ef, 0x7fb, 0x7ff, 0x7fb, 0x7ef, 0x7db, 0x7bf,
        0x79b, 0x76f, 0x73c, 0x702, 0x6c0, 0x678, 0x629, 0x5d4, 0x579, 0x519,
        0x4b3, 0x449, 0x3da, 0x367, 0x2f1, 0x278, 0x1fd, 0x17f, 0x100, 0x080,

        -0x0, -0x080, -0x100, -0x17f, -0x1fd, -0x278, -0x2f1, -0x367, -0x3da, -0x449,
        -0x4b3, -0x519, -0x579, -0x5d4, -0x629, -0x678, -0x6c0, -0x702, -0x73c, -0x76f,
        -0x79b, -0x7bf, -0x7db, -0x7ef, -0x7fb, -0x7ff, -0x7fb, -0x7ef, -0x7db, -0x7bf,
        -0x79b, -0x76f, -0x73c, -0x702, -0x6c0, -0x678, -0x629, -0x5d4, -0x579, -0x519,
        -0x4b3, -0x449, -0x3da, -0x367, -0x2f1, -0x278, -0x1fd, -0x17f, -0x100, -0x080
    };

    enum : uint32_t {
        Sample_count     = sizeof(Sin_table) / sizeof(Sin_table[0]),
        DAC_resolution   = 12,
        Max_data         = ((1 << DAC_resolution) - 1),
        Amplitude_offset = Max_data / 2,
        Max_digital      = 0x7ff * 2,
    };

    static uint8_t the_wave_type{};
    static int32_t the_amplitude{Amplitude_offset};
    static uint32_t the_sample_index{};

    static void next_sample() {
        the_sample_index++;
        if (the_sample_index >= Sample_count) {
            the_sample_index = 0;
        }
    }

    static uint32_t sin_wave() {
        auto wave = Sin_table[the_sample_index];
        return static_cast<int>(wave) * the_amplitude / Max_digital + Amplitude_offset;
    }

    static uint32_t square_wave() {
        return ((the_sample_index > Sample_count / 2) ? 0 : Amplitude_offset);
    }
}

extern "C" {
void ISR_DACC() {
    if (auto *driver = SamV71_DAC_definition::optional_DAC) {
        driver->ISR();
    } else {
        NVIC_DisableIRQ(DACC_IRQn);
        NVIC_ClearPendingIRQ(DACC_IRQn);
    }
}
}

namespace SamV71 {
    using namespace SamV71_DAC_definition;

    SamV71_DAC::SamV71_DAC() : optional_definition(get_definition(0)) {
        configASSERT(optional_definition != nullptr);
    }

    void SamV71_DAC::initialize() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            auto channel = static_cast<Definition *>(optional_definition)->the_channel;
            auto peripheral_id = static_cast<Definition *>(optional_definition)->the_IRQ_number;
            SamV71_clock::enable_peripheral_clock(peripheral_id);
            base->DACC_CR = DACC_CR_SWRST(1);         // Reset
            base->DACC_MR = 0;                        // Transfer mode
            base->DACC_CHER = DACC_CHER_CH0(channel); // Enable channel
            base->DACC_ACR = Analog_control;
        }
    }

    bool SamV71_DAC::set(const uint32_t value) {
        auto success{false};
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            auto channel = static_cast<Definition *>(optional_definition)->the_channel;
            if ((base->DACC_CHER & DACC_CHSR_DACRDY0(channel) && base->DACC_CHER & DACC_CHSR_CH(channel))) {
                base->DACC_CDR[channel] = value; // Write-only FIFO register
                the_DAC_setting = value; // Save most recent setting
                success = true;
            }
        }
        return success;
    }

    uint32_t SamV71_DAC::get() const {
        return the_DAC_setting;
    }

    void SamV71_DAC::ISR() {
        if (optional_definition) {
            auto *base = static_cast<Definition *>(optional_definition)->the_base;
            auto status = base->DACC_ISR;
            if ((status & DACC_ISR_TXRDY0_Msk) == DACC_ISR_TXRDY0(1)) {
                ISR_ready();
            }
        }
    }

    void SamV71_DAC::ISR_ready() {
        using namespace SamV71_DACC_test_data;
        next_sample();
        auto dac_val = the_wave_type ? square_wave() : sin_wave();
        SamV71_DAC::set(dac_val);
    }
} // SamV71
