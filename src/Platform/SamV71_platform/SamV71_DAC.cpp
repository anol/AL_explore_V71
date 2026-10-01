/*
* Copyright (C) 2026 Integrated Detector Electronics AS
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
* @file   SamV71_DAC.cpp
* @author AndersEmilOlsen, IDEAS
* @date   01.10.2026
* @brief  
*/

#include <cstdint>
#include "SamV71_DAC.h"

#include "sam.h"
#include "component/dacc.h"

#define wave_to_dacc(wave, amplitude, max_digital, max_amplitude) \
	(((int)(wave) * (amplitude) / (max_digital)) + (max_amplitude / 2))

#define MAX_DIGITAL   (0x7ff)

#define MAX_AMPLITUDE (DACC_MAX_DATA)

#define DACC_CHANNEL 0
#define DACC_WP_KEY     (0x444143)
#define DACC_RESOLUTION     12
#define DACC_MAX_DATA       ((1 << DACC_RESOLUTION) - 1)
#define MAX_CH_NB        1
#define SAMPLES (100)
#define DACC_BASE DACC_REGS
#define DACC_ANALOG_CONTROL (DACC_ACR_IBCTLCH0(0x02) | DACC_ACR_IBCTLCH1(0x02))
/** Waveform selector */
uint8_t g_uc_wave_sel = 0;

/** 100 points of sinewave samples, amplitude is MAX_DIGITAL*2 */
const int16_t gc_us_sine_data[SAMPLES] = {
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

namespace SamV71 {
    void dacc_reset(dacc_registers_t *p_dacc) {
        p_dacc->DACC_CR = DACC_CR_SWRST(1);
    }

    void dacc_set_transfer_mode(dacc_registers_t *p_dacc, uint32_t ul_mode) {
        if (ul_mode) {
            p_dacc->DACC_MR = ul_mode;
        } else {
            p_dacc->DACC_MR = ul_mode;
        }
    }

    void dacc_enable_channel(dacc_registers_t *p_dacc, uint32_t ul_channel) {
        if (ul_channel > MAX_CH_NB) return;
        p_dacc->DACC_CHER = DACC_CHER_CH0(ul_channel);
    }

    void dacc_set_analog_control(dacc_registers_t *p_dacc, uint32_t ul_analog_control) {
        p_dacc->DACC_ACR = ul_analog_control;
    }

    uint32_t dacc_get_interrupt_status(dacc_registers_t *p_dacc) {
        return p_dacc->DACC_ISR;
    }

    void dacc_write_conversion_data(dacc_registers_t *p_dacc, uint32_t ul_data, uint32_t channel) {
        p_dacc->DACC_CDR[channel] = ul_data;
    }

    void SamV71_DAC_initialize() {
        // configure IO
        // sysclk_enable_peripheral_clock(DACC_ID);
        dacc_reset(DACC_BASE);
        dacc_set_transfer_mode(DACC_BASE, 0);
        dacc_enable_channel(DACC_BASE, DACC_CHANNEL);
        dacc_set_analog_control(DACC_BASE, DACC_ANALOG_CONTROL);
    }

    void on() {
        static int32_t g_l_amplitude = 0;
        static uint32_t g_ul_index_sample = 0;
        uint32_t status = dacc_get_interrupt_status(DACC_BASE);
        if ((status & DACC_ISR_TXRDY0_Msk) == DACC_ISR_TXRDY0(1)) {
            g_ul_index_sample++;
            if (g_ul_index_sample >= SAMPLES) {
                g_ul_index_sample = 0;
            }
            uint32_t dac_val =
                    g_uc_wave_sel
                        ? ((g_ul_index_sample > SAMPLES / 2) ? 0 : MAX_AMPLITUDE)
                        : wave_to_dacc(gc_us_sine_data[g_ul_index_sample],
                                       g_l_amplitude,
                                       MAX_DIGITAL * 2, MAX_AMPLITUDE);
            dacc_write_conversion_data(DACC_BASE, dac_val, DACC_CHANNEL);
        }
    }
} // SamV71
