module;

#include <cstdio>

#include "sam.h"
#include "component/afec.h"

module Platform.SamV71_ADC;
import Type.Abstract_ADC;

#define SAMV71 true
#define AFEC0 AFEC0_REGS
#define AFEC1 AFEC1_REGS

/** Reference voltage for AFEC,in mv. */
#define VOLT_REF        (3300)

/** The maximal digital value */
#define MAX_DIGITAL     (4095UL)

namespace {
    enum afec_resolution {
        AFEC_12_BITS = AFEC_EMR_RES_NO_AVERAGE, /* AFEC 12-bit resolution */
        AFEC_13_BITS = AFEC_EMR_RES_OSR4,       /* AFEC 13-bit resolution */
        AFEC_14_BITS = AFEC_EMR_RES_OSR16,      /* AFEC 14-bit resolution */
        AFEC_15_BITS = AFEC_EMR_RES_OSR64,      /* AFEC 15-bit resolution */
        AFEC_16_BITS = AFEC_EMR_RES_OSR256      /* AFEC 16-bit resolution */
    };

    /** Definitions for AFEC Start Up Time */
    enum afec_startup_time {
        AFEC_STARTUP_TIME_0  = AFEC_MR_STARTUP_SUT0,
        AFEC_STARTUP_TIME_1  = AFEC_MR_STARTUP_SUT8,
        AFEC_STARTUP_TIME_2  = AFEC_MR_STARTUP_SUT16,
        AFEC_STARTUP_TIME_3  = AFEC_MR_STARTUP_SUT24,
        AFEC_STARTUP_TIME_4  = AFEC_MR_STARTUP_SUT64,
        AFEC_STARTUP_TIME_5  = AFEC_MR_STARTUP_SUT80,
        AFEC_STARTUP_TIME_6  = AFEC_MR_STARTUP_SUT96,
        AFEC_STARTUP_TIME_7  = AFEC_MR_STARTUP_SUT112,
        AFEC_STARTUP_TIME_8  = AFEC_MR_STARTUP_SUT512,
        AFEC_STARTUP_TIME_9  = AFEC_MR_STARTUP_SUT576,
        AFEC_STARTUP_TIME_10 = AFEC_MR_STARTUP_SUT640,
        AFEC_STARTUP_TIME_11 = AFEC_MR_STARTUP_SUT704,
        AFEC_STARTUP_TIME_12 = AFEC_MR_STARTUP_SUT768,
        AFEC_STARTUP_TIME_13 = AFEC_MR_STARTUP_SUT832,
        AFEC_STARTUP_TIME_14 = AFEC_MR_STARTUP_SUT896,
        AFEC_STARTUP_TIME_15 = AFEC_MR_STARTUP_SUT960
    };

    enum afec_trigger {
        /* Starting a conversion is only possible by software. */
        AFEC_TRIG_SW               = AFEC_MR_TRGEN_DIS,
        /* External trigger */
        AFEC_TRIG_EXT              = AFEC_MR_TRGSEL_AFEC_TRIG0 | AFEC_MR_TRGEN_Msk,
        /* TIO Output of the Timer Counter Channel 0 */
        AFEC_TRIG_TIO_CH_0         = AFEC_MR_TRGSEL_AFEC_TRIG1 | AFEC_MR_TRGEN_Msk,
        /* TIO Output of the Timer Counter Channel 1 */
        AFEC_TRIG_TIO_CH_1         = AFEC_MR_TRGSEL_AFEC_TRIG2 | AFEC_MR_TRGEN_Msk,
        /* TIO Output of the Timer Counter Channel 2 */
        AFEC_TRIG_TIO_CH_2         = AFEC_MR_TRGSEL_AFEC_TRIG3 | AFEC_MR_TRGEN_Msk,
        /* PWM Event Line 0 */
        AFEC_TRIG_PWM_EVENT_LINE_0 = AFEC_MR_TRGSEL_AFEC_TRIG4 | AFEC_MR_TRGEN_Msk,
        /* PWM Event Line 1 */
        AFEC_TRIG_PWM_EVENT_LINE_1 = AFEC_MR_TRGSEL_AFEC_TRIG5 | AFEC_MR_TRGEN_Msk,
#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
        /*Analog Comparator*/
        AFEC_TRIG_ANALOG_COMPARATOR = AFEC_MR_TRGSEL_AFEC_TRIG6 | AFEC_MR_TRGEN_Msk,
#endif
        /* Freerun mode conversion. */
        AFEC_TRIG_FREERUN = 0xFF,
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

    enum afec_channel_num {
        AFEC_CHANNEL_0 = 0,
        AFEC_CHANNEL_1,
        AFEC_CHANNEL_2,
        AFEC_CHANNEL_3,
        AFEC_CHANNEL_4,
        AFEC_CHANNEL_5,
        AFEC_CHANNEL_6,
        AFEC_CHANNEL_7,
        AFEC_CHANNEL_8,
        AFEC_CHANNEL_9,
        AFEC_CHANNEL_10,
        AFEC_TEMPERATURE_SENSOR,
        AFEC_CHANNEL_ALL = 0x0FFF,
    };

#define NUM_OF_AFEC    (2UL)

    typedef void (*afec_callback_t)(void);

    afec_callback_t afec_callback_pointer[NUM_OF_AFEC][_AFEC_NUM_OF_INTERRUPT_SOURCE];

    struct afec_config {
        /** Resolution */
        enum afec_resolution resolution;
        /** Master Clock */
        uint32_t mck;
        /** AFEC Clock */
        uint32_t afec_clock;
        /** Start Up Time */
        enum afec_startup_time startup_time;
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
}

namespace SamV71 {
    static uint32_t afec_find_pid(afec_registers_t *const afec) {
        if (afec == AFEC1_REGS) {
            return ID_AFEC1;
        }
        if (afec == AFEC0_REGS) {
            return ID_AFEC0;
        }
        return ID_AFEC0;
    }

    void afec_enable(afec_registers_t *const afec) {
        uint32_t pid = afec_find_pid(afec);
        /* Enable peripheral clock. */
        // pmc_enable_periph_clk(pid);
        // sleepmgr_lock_mode(SLEEPMGR_SLEEP_WFI);
    }

    void afec_get_config_defaults(afec_config *const cfg) {
        cfg->resolution = AFEC_12_BITS;
        cfg->mck = 150'000'000; // sysclk_get_cpu_hz();
        cfg->afec_clock = 6000000UL;
        cfg->startup_time = AFEC_STARTUP_TIME_4;
        cfg->tracktim = 2;
        cfg->transfer = 1;
        cfg->anach = true;
        cfg->useq = false;
        cfg->tag = true;
        cfg->stm = true;
        cfg->ibctl = 1;
    }

    static void afec_set_config(afec_registers_t *const afec, struct afec_config *config) {
        uint32_t reg = 0;
        reg = (config->useq ? AFEC_MR_USEQ_REG_ORDER : 0) |
              AFEC_MR_PRESCAL((config->mck / config->afec_clock )- 1) |
              AFEC_MR_ONE_Msk |
              AFEC_MR_TRACKTIM(config->tracktim) |
              AFEC_MR_TRANSFER(config->transfer) |
              (config->startup_time);
        afec->AFEC_MR = reg;
        afec->AFEC_EMR = (config->tag ? AFEC_EMR_TAG_Msk : 0) |
                         (config->resolution) |
                         (config->stm ? AFEC_EMR_STM_Msk : 0);
        afec->AFEC_ACR = AFEC_ACR_IBCTL(config->ibctl) | AFEC_ACR_PGA0EN_Msk | AFEC_ACR_PGA1EN_Msk;
    }

    static void afec_interrupt(uint8_t inst_num,
                               enum afec_interrupt_source source) {
        if (afec_callback_pointer[inst_num][source]) {
            afec_callback_pointer[inst_num][source]();
        }
    }

    static uint32_t afec_get_interrupt_status(afec_registers_t *const afec) {
        return afec->AFEC_ISR;
    }

    static void afec_init(afec_registers_t *afec, afec_config *config) {
        if ((afec_get_interrupt_status(afec) & AFEC_ISR_DRDY_Msk) == AFEC_ISR_DRDY_Msk) {
            return;
        }
        /* Reset and configure the AFEC module */
        afec->AFEC_CR = AFEC_CR_SWRST_Msk;
        afec_set_config(afec, config);
        uint32_t i;
        if (afec == AFEC0) {
            for (i = 0; i < _AFEC_NUM_OF_INTERRUPT_SOURCE; i++) {
                afec_callback_pointer[0][i] = 0;
            }
        }
        if (afec == AFEC1) {
            for (i = 0; i < _AFEC_NUM_OF_INTERRUPT_SOURCE; i++) {
                afec_callback_pointer[1][i] = 0;
            }
        }
    }

    static void afec_set_trigger(afec_registers_t *const afec,
                                 const enum afec_trigger trigger) {
        uint32_t reg;

        reg = afec->AFEC_MR;

        if (trigger == AFEC_TRIG_FREERUN) {
            reg |= AFEC_MR_FREERUN_ON;
        } else {
            reg &= ~(AFEC_MR_TRGSEL_Msk | AFEC_MR_TRGEN_Msk | AFEC_MR_FREERUN_ON);
            reg |= trigger;
        }

        afec->AFEC_MR = reg;
    }

    static void afec_ch_get_config_defaults(struct afec_ch_config *const cfg) {
        cfg->diff = false;
        cfg->gain = AFEC_GAINVALUE_1;
    }

    static void afec_ch_set_config(afec_registers_t *const afec, const enum afec_channel_num channel,
                                   struct afec_ch_config *config) {
        uint32_t reg = 0;

        reg = afec->AFEC_DIFFR;
        reg &= ~(0x1u << channel);
        reg |= (config->diff) ? (0x1u << channel) : 0;
        afec->AFEC_DIFFR = reg;

        reg = afec->AFEC_CGR;
        reg &= ~(0x03u << (2 * channel));
        reg |= (config->gain) << (2 * channel);
        afec->AFEC_CGR = reg;
    }

    static void afec_channel_set_analog_offset(afec_registers_t *const afec,
                                               enum afec_channel_num afec_ch, uint16_t aoffset) {
        afec->AFEC_CSELR = afec_ch;
        afec->AFEC_COCR = (aoffset & AFEC_COCR_AOFF_Msk);
    }

    static void afec_temp_sensor_get_config_defaults(
        struct afec_temp_sensor_config *const cfg) {
        cfg->rctc = false;
        cfg->mode = AFEC_TEMP_CMP_MODE_2;
        cfg->low_threshold = 0xFF;
        cfg->high_threshold = 0xFFF;
    }

    static void afec_temp_sensor_set_config(afec_registers_t *const afec,
                                            struct afec_temp_sensor_config *config) {
        uint32_t reg = ((config->rctc) ? AFEC_TEMPMR_RTCT_Msk : 0) | (config->mode);
        afec->AFEC_TEMPMR = reg;
        afec->AFEC_TEMPCWR = AFEC_TEMPCWR_TLOWTHRES(config->low_threshold) |
                             AFEC_TEMPCWR_THIGHTHRES(config->high_threshold);
    }

    uint32_t afec_find_inst_num(afec_registers_t *const afec) {
        if (afec == AFEC1) {
            return 1;
        }
        if (afec == AFEC0) {
            return 0;
        }
        return 0;
    }

    /* The interrupt source number of temperature sensor */
#define AFEC_TEMP_INT_SOURCE_NUM             (11UL)
#define AFEC_INTERRUPT_GAP1                  (12UL)
#define AFEC_INTERRUPT_GAP2                  (3UL)

    void afec_enable_interrupt(afec_registers_t *const afec,
                               enum afec_interrupt_source interrupt_source) {
        if (interrupt_source == AFEC_INTERRUPT_ALL) {
            afec->AFEC_IER = AFEC_INTERRUPT_ALL;
            return;
        }
        if (interrupt_source < AFEC_INTERRUPT_DATA_READY) {
            if (interrupt_source == AFEC_INTERRUPT_EOC_11) {
                afec->AFEC_IER = 1 << AFEC_TEMP_INT_SOURCE_NUM;
            } else {
                afec->AFEC_IER = 1 << interrupt_source;
            }
        } else if (interrupt_source < AFEC_INTERRUPT_TEMP_CHANGE) {
            afec->AFEC_IER = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1);
        } else {
            afec->AFEC_IER = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1 + AFEC_INTERRUPT_GAP2);
        }
    }

    void afec_set_callback(afec_registers_t *const afec, enum afec_interrupt_source source,
                           afec_callback_t callback, uint8_t irq_level) {
        uint32_t i = afec_find_inst_num(afec);
        afec_callback_pointer[i][source] = callback;
        if (!i) {
            // irq_register_handler(AFEC0_IRQn, irq_level);
        } else if (i == 1) {
            // irq_register_handler(AFEC1_IRQn, irq_level);
        }
        /* Enable the specified interrupt source */
        afec_enable_interrupt(afec, source);
    }

    uint32_t afec_channel_get_value(afec_registers_t *const afec,
                                    enum afec_channel_num afec_ch) {
        afec->AFEC_CSELR = afec_ch;
        return afec->AFEC_CDR;
    }

    volatile bool is_conversion_done = false;

    volatile uint32_t g_ul_value = 0;

    void afec_temp_sensor_end_conversion() {
        g_ul_value = afec_channel_get_value(AFEC0, AFEC_TEMPERATURE_SENSOR);
        is_conversion_done = true;
    }

    void go() {
        int32_t ul_vol;
        int32_t ul_temp;
        while (1) {
            if (is_conversion_done == true) {
                ul_vol = g_ul_value * VOLT_REF / MAX_DIGITAL;

                /*
                * According to datasheet, The output voltage VT = 0.72V at 27C
                * and the temperature slope dVT/dT = 2.33 mV/C
                */
                ul_temp = (ul_vol - 720) * 100 / 233 + 27;
                std::printf("Temperature is: %4d\r", (int) ul_temp);
                is_conversion_done = false;
            }
        }
    }

    SamV71_ADC::SamV71_ADC() {
    }

    void SamV71_ADC::initialize() {
        afec_enable(AFEC0);
        afec_config afec_cfg{};
        afec_get_config_defaults(&afec_cfg);
        afec_init(AFEC0, &afec_cfg);
        afec_set_trigger(AFEC0, AFEC_TRIG_SW);
        afec_ch_config afec_ch_cfg{};
        afec_ch_get_config_defaults(&afec_ch_cfg);
        afec_ch_cfg.gain = AFEC_GAINVALUE_0;
        afec_ch_set_config(AFEC0, AFEC_TEMPERATURE_SENSOR, &afec_ch_cfg);
        afec_channel_set_analog_offset(AFEC0, AFEC_TEMPERATURE_SENSOR, 0x200);
        afec_temp_sensor_config afec_temp_sensor_cfg{};
        afec_temp_sensor_get_config_defaults(&afec_temp_sensor_cfg);
        afec_temp_sensor_cfg.rctc = true;
        afec_temp_sensor_set_config(AFEC0, &afec_temp_sensor_cfg);
        afec_set_callback(AFEC0, AFEC_INTERRUPT_EOC_11, afec_temp_sensor_end_conversion, 1);
    }

    bool SamV71_ADC::get(uint32_t &value) {
        value = g_ul_value;
        return true;
    }
} // SamV71
