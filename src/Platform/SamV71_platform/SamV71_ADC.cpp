module;

#include <cstdio>

#include "sam.h"
#include "component/afec.h"

module Platform.SamV71_ADC;
import Type.Abstract_ADC;
import Platform.SamV71_clock;

#define SAMV71 true
#define AFEC0 AFEC0_REGS
#define AFEC1 AFEC1_REGS

/** Reference voltage for AFEC,in mv. */
#define VOLT_REF        (3300)

/** The maximal digital value */
#define MAX_DIGITAL     (4095UL)

namespace {
    enum afec_trigger {
        /* Starting a conversion is only possible by software. */
        AFEC_TRIG_SW                = AFEC_MR_TRGEN_DIS,
        /* External trigger */
        AFEC_TRIG_EXT               = AFEC_MR_TRGSEL_AFEC_TRIG0 | AFEC_MR_TRGEN_Msk,
        /* TIO Output of the Timer Counter Channel 0 */
        AFEC_TRIG_TIO_CH_0          = AFEC_MR_TRGSEL_AFEC_TRIG1 | AFEC_MR_TRGEN_Msk,
        /* TIO Output of the Timer Counter Channel 1 */
        AFEC_TRIG_TIO_CH_1          = AFEC_MR_TRGSEL_AFEC_TRIG2 | AFEC_MR_TRGEN_Msk,
        /* TIO Output of the Timer Counter Channel 2 */
        AFEC_TRIG_TIO_CH_2          = AFEC_MR_TRGSEL_AFEC_TRIG3 | AFEC_MR_TRGEN_Msk,
        /* PWM Event Line 0 */
        AFEC_TRIG_PWM_EVENT_LINE_0  = AFEC_MR_TRGSEL_AFEC_TRIG4 | AFEC_MR_TRGEN_Msk,
        /* PWM Event Line 1 */
        AFEC_TRIG_PWM_EVENT_LINE_1  = AFEC_MR_TRGSEL_AFEC_TRIG5 | AFEC_MR_TRGEN_Msk,
        AFEC_TRIG_ANALOG_COMPARATOR = AFEC_MR_TRGSEL_AFEC_TRIG6 | AFEC_MR_TRGEN_Msk,
        /* Freerun mode conversion. */
        AFEC_TRIG_FREERUN           = 0xFF,
    };

    enum afec_interrupt_source {
        AFEC_INTERRUPT_EOC_0 = 0,
        AFEC_INTERRUPT_EOC_1,
        AFEC_INTERRUPT_EOC_2,
        AFEC_INTERRUPT_EOC_3,
        AFEC_INTERRUPT_EOC_4,
        AFEC_INTERRUPT_EOC_5,
        AFEC_INTERRUPT_EOC_6,
        AFEC_INTERRUPT_EOC_7,
        AFEC_INTERRUPT_EOC_8,
        AFEC_INTERRUPT_EOC_9,
        AFEC_INTERRUPT_EOC_10,
        AFEC_INTERRUPT_EOC_11,
        AFEC_INTERRUPT_DATA_READY,
        AFEC_INTERRUPT_OVERRUN_ERROR,
        AFEC_INTERRUPT_COMP_ERROR,
        AFEC_INTERRUPT_TEMP_CHANGE,
        _AFEC_NUM_OF_INTERRUPT_SOURCE,
        AFEC_INTERRUPT_ALL = 0x47000FFF,
    };

    constexpr uint32_t NUM_OF_AFEC = 2UL;

    typedef void (*afec_callback_t)();

    afec_callback_t afec_callback_pointer[NUM_OF_AFEC][_AFEC_NUM_OF_INTERRUPT_SOURCE];

    struct afec_config {
        /** Resolution */
        unsigned long resolution;
        /** Master Clock */
        uint32_t mck;
        /** AFEC Clock */
        uint32_t afec_clock;
        /** Start Up Time */
        unsigned long startup_time;
        /** Tracking Time = tracktim / AFEC clock */
        uint8_t tracktim;
        /** Transfer Period = (transfer * 2 + 3) / AFEC clock */
        uint8_t transfer;
        /** Analog Change */
        bool anach;
        /** Use Sequence Enable */
        bool useq;
        /** TAG of AFE_LDCR register */
        bool tag;
        /** Single Trigger Mode */
        bool stm;
        /** AFE Bias Current Control */
        uint8_t ibctl;
    };

    /** Definitions for AFEC gain value */
    enum afec_gainvalue {
        AFEC_GAINVALUE_0 = 0,
        AFEC_GAINVALUE_1 = 1,
        AFEC_GAINVALUE_2 = 2,
        AFEC_GAINVALUE_3 = 3
    };

    /** AFEC channel configuration structure.*/
    struct afec_ch_config {
        /** Differential Mode */
        bool diff;
        /** Gain Value */
        enum afec_gainvalue gain;
    };

    /** Definitions for Temperature Comparison Mode */
    enum afec_temp_cmp_mode {
        AFEC_TEMP_CMP_MODE_0 = AFEC_TEMPMR_TEMPCMPMOD_LOW,
        AFEC_TEMP_CMP_MODE_1 = AFEC_TEMPMR_TEMPCMPMOD_HIGH,
        AFEC_TEMP_CMP_MODE_2 = AFEC_TEMPMR_TEMPCMPMOD_IN,
        AFEC_TEMP_CMP_MODE_3 = AFEC_TEMPMR_TEMPCMPMOD_OUT
    };

    /** AFEC Temperature Sensor configuration structure.*/
    struct afec_temp_sensor_config {
        /** RTC Trigger mode */
        bool rctc;
        /** Temperature Comparison Mode */
        enum afec_temp_cmp_mode mode;
        /** Temperature Low Threshold */
        uint16_t low_threshold;
        /** Temperature High Threshold */
        uint16_t high_threshold;
    };

    /* The interrupt source number of temperature sensor */
    constexpr uint32_t AFEC_TEMP_INT_SOURCE_NUM = (11UL);
    constexpr uint32_t AFEC_INTERRUPT_GAP1 = (12UL);
    constexpr uint32_t AFEC_INTERRUPT_GAP2 = (3UL);
}

namespace SamV71 {
    static void afec_get_config_defaults(afec_config *const cfg) {
        cfg->resolution = AFEC_EMR_RES_NO_AVERAGE; // AFEC_12_BITS;
        cfg->mck = 150'000'000;                    // sysclk_get_cpu_hz();
        cfg->afec_clock = 6000000UL;
        cfg->startup_time = AFEC_MR_STARTUP_SUT64;
        cfg->tracktim = 2;
        cfg->transfer = 1;
        cfg->anach = true;
        cfg->useq = false;
        cfg->tag = true;
        cfg->stm = true;
        cfg->ibctl = 1;
    }

    // static void afec_interrupt(uint8_t inst_num,
    //                            enum afec_interrupt_source source) {
    //     if (afec_callback_pointer[inst_num][source]) {
    //         afec_callback_pointer[inst_num][source]();
    //     }
    // }

    static uint32_t afec_get_interrupt_status(afec_registers_t *const afec) {
        return afec->AFEC_ISR;
    }

    static void afec_ch_get_config_defaults(afec_ch_config *const cfg) {
        cfg->diff = false;
        cfg->gain = AFEC_GAINVALUE_1;
    }

    static void afec_temp_sensor_get_config_defaults(afec_temp_sensor_config *const cfg) {
        cfg->rctc = false;
        cfg->mode = AFEC_TEMP_CMP_MODE_2;
        cfg->low_threshold = 0xFF;
        cfg->high_threshold = 0xFFF;
    }

    static void afec_temp_sensor_set_config(afec_registers_t *const optional_base, const afec_temp_sensor_config *config) {
        const uint32_t reg = ((config->rctc) ? AFEC_TEMPMR_RTCT_Msk : 0) | (config->mode);
        optional_base->AFEC_TEMPMR = reg;
        optional_base->AFEC_TEMPCWR = AFEC_TEMPCWR_TLOWTHRES(config->low_threshold) | AFEC_TEMPCWR_THIGHTHRES(config->high_threshold);
    }

    static uint32_t afec_find_inst_num(const afec_registers_t *const optional_base) {
        if (optional_base == AFEC1) {
            return 1;
        }
        if (optional_base == AFEC0) {
            return 0;
        }
        return 0;
    }

    static void afec_enable_interrupt(afec_registers_t *const optional_base, afec_interrupt_source interrupt_source) {
        if (interrupt_source == AFEC_INTERRUPT_ALL) {
            optional_base->AFEC_IER = AFEC_INTERRUPT_ALL;
            return;
        }
        if (interrupt_source < AFEC_INTERRUPT_DATA_READY) {
            if (interrupt_source == AFEC_INTERRUPT_EOC_11) {
                optional_base->AFEC_IER = 1 << AFEC_TEMP_INT_SOURCE_NUM;
            } else {
                optional_base->AFEC_IER = 1 << interrupt_source;
            }
        } else if (interrupt_source < AFEC_INTERRUPT_TEMP_CHANGE) {
            optional_base->AFEC_IER = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1);
        } else {
            optional_base->AFEC_IER = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1 + AFEC_INTERRUPT_GAP2);
        }
    }

    static void afec_set_callback(afec_registers_t *const optional_base, const afec_interrupt_source source,
                                  const afec_callback_t callback, uint8_t irq_level) {
        uint32_t i = afec_find_inst_num(optional_base);
        afec_callback_pointer[i][source] = callback;
        if (!i) {
            // irq_register_handler(AFEC0_IRQn, irq_level);
        } else if (i == 1) {
            // irq_register_handler(AFEC1_IRQn, irq_level);
        }
        /* Enable the specified interrupt source */
        afec_enable_interrupt(optional_base, source);
    }

    static uint32_t afec_channel_get_value(afec_registers_t *const optional_base, uint16_t afec_ch) {
        optional_base->AFEC_CSELR = afec_ch;
        return optional_base->AFEC_CDR;
    }

    static volatile bool is_conversion_done = false;

    static volatile uint32_t g_ul_value = 0;

    static void afec_temp_sensor_end_conversion() {
        g_ul_value = afec_channel_get_value(AFEC0, SamV71_ADC::AFEC_TEMPERATURE_SENSOR);
        is_conversion_done = true;
    }

    void go() {
        while (true) {
            if (is_conversion_done == true) {
                int32_t ul_vol = g_ul_value * VOLT_REF / MAX_DIGITAL;
                // According to datasheet, The output voltage VT = 0.72V at 27C
                // and the temperature slope dVT/dT = 2.33 mV/C
                int32_t ul_temp = (ul_vol - 720) * 100 / 233 + 27;
                std::printf("Temperature is: %4d\r", (int) ul_temp);
                is_conversion_done = false;
            }
        }
    }

    void SamV71_ADC::configure_temp_sensor() {
        afec_temp_sensor_config config{};
        afec_temp_sensor_get_config_defaults(&config);
        config.rctc = true;
        afec_temp_sensor_set_config(optional_base, &config);
    }

    void SamV71_ADC::initialize() {
        SamV71_clock::enable_peripheral_clock(peripheral_id);
        configure_AFEC();
        set_trigger(AFEC_MR_TRGEN_DIS); // AFEC_TRIG_SW
        configure_channel(AFEC_TEMPERATURE_SENSOR, 0x200);
        configure_temp_sensor();
        afec_set_callback(optional_base, AFEC_INTERRUPT_EOC_11, afec_temp_sensor_end_conversion, 1);
    }

    void SamV71_ADC::configure_AFEC() {
        if ((afec_get_interrupt_status(optional_base) & AFEC_ISR_DRDY_Msk) == AFEC_ISR_DRDY_Msk) {
            return;
        }
        optional_base->AFEC_CR = AFEC_CR_SWRST(1); // Reset control
        optional_base->AFEC_MR = 0;                // Reset mode
        afec_config config{};
        afec_get_config_defaults(&config);
        uint32_t reg = (config.useq ? AFEC_MR_USEQ_REG_ORDER : 0) |
                       AFEC_MR_PRESCAL((config.mck / config.afec_clock )- 1) |
                       AFEC_MR_ONE_Msk |
                       AFEC_MR_TRACKTIM(config.tracktim) |
                       AFEC_MR_TRANSFER(config.transfer) |
                       (config.startup_time);
        optional_base->AFEC_MR = reg;
        optional_base->AFEC_EMR = (config.tag ? AFEC_EMR_TAG_Msk : 0) |
                                  (config.resolution) |
                                  (config.stm ? AFEC_EMR_STM_Msk : 0);
        optional_base->AFEC_ACR = AFEC_ACR_IBCTL(config.ibctl) | AFEC_ACR_PGA0EN_Msk | AFEC_ACR_PGA1EN_Msk;
        uint32_t i;
        if (optional_base == AFEC0) {
            for (i = 0; i < _AFEC_NUM_OF_INTERRUPT_SOURCE; i++) {
                afec_callback_pointer[0][i] = 0;
            }
        }
        if (optional_base == AFEC1) {
            for (i = 0; i < _AFEC_NUM_OF_INTERRUPT_SOURCE; i++) {
                afec_callback_pointer[1][i] = 0;
            }
        }
    }

    void SamV71_ADC::set_trigger(const uint32_t trigger) {
        uint32_t reg = optional_base->AFEC_MR;
        if (trigger == AFEC_TRIG_FREERUN) {
            reg |= AFEC_MR_FREERUN_ON;
        } else {
            reg &= ~(AFEC_MR_TRGSEL_Msk | AFEC_MR_TRGEN_Msk | AFEC_MR_FREERUN_ON);
            reg |= trigger;
        }
        optional_base->AFEC_MR = reg;
    }

    void SamV71_ADC::configure_channel(const Channel_number channel, const uint16_t channel_offset) {
        afec_ch_config config{};
        afec_ch_get_config_defaults(&config);
        config.gain = AFEC_GAINVALUE_0;
        uint32_t reg = optional_base->AFEC_DIFFR;
        reg &= ~(0x1u << channel);
        reg |= (config.diff) ? (0x1u << channel) : 0;
        optional_base->AFEC_DIFFR = reg;
        reg = optional_base->AFEC_CGR;
        reg &= ~(0x03u << (2 * channel));
        reg |= (config.gain) << (2 * channel);
        optional_base->AFEC_CGR = reg;
        optional_base->AFEC_CSELR = channel;
        optional_base->AFEC_COCR = (channel_offset & AFEC_COCR_AOFF_Msk);
    }

    bool SamV71_ADC::get(uint32_t &value) {
        value = g_ul_value;
        return true;
    }
} // SamV71
