/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 *
 */

/**
 * \date   IDEAS/11.02.2020/aeols
 * \brief
 */

#include <cstdint>
#include <Common/NORM_interface_definition.h>
#include <include/samv71q21b_symbols.h>
#include <include/nvic_helpers.h>
#include <Service_transaction/Service_report.h>
#include "SAMV71_clock.h"
#include "sysclk.h"

volatile uint32_t Clock_interface::milliseconds_since_start = 0;

uint32_t Clock_interface::get_millisecond_timestamp() { return Clock_interface::milliseconds_since_start; }

void TC0_Handler_SAMV71_clock() {
    TC0->TC_CHANNEL[0].TC_SR;
    Clock_interface::milliseconds_since_start++;
}

uint32_t Clock_interface::get_milliseconds() {
    return milliseconds_since_start;
}

//    finput = 12 MHz
//    MAINCK = 12 MHz
//    PLLACK = MAINCK * MULA / DIVA = (12 * (0 × 18 + 1) / 1) MHz = 300 MHz
//    HCLK = PLLACK / PRES = (300 / 1) MHz = 300 MHz
//    MCK = PLLACK / PRES / MDIV = (300 / 1 / 2) MHz = 150 MHz


static void setup_embedded_flash(uint32_t ul_clk) {
    /* Set FWS for embedded Flash access according to operating frequency */
    if (ul_clk < CHIP_FREQ_FWS_0) {
        EFC->EEFC_FMR = EEFC_FMR_FWS(0u) | EEFC_FMR_CLOE;
    } else {
        if (ul_clk < CHIP_FREQ_FWS_1) {
            EFC->EEFC_FMR = EEFC_FMR_FWS(1u) | EEFC_FMR_CLOE;
        } else {
            if (ul_clk < CHIP_FREQ_FWS_2) {
                EFC->EEFC_FMR = EEFC_FMR_FWS(2u) | EEFC_FMR_CLOE;
            } else {
                if (ul_clk < CHIP_FREQ_FWS_3) {
                    EFC->EEFC_FMR = EEFC_FMR_FWS(3u) | EEFC_FMR_CLOE;
                } else {
                    if (ul_clk < CHIP_FREQ_FWS_4) {
                        EFC->EEFC_FMR = EEFC_FMR_FWS(4u) | EEFC_FMR_CLOE;
                    } else {
                        if (ul_clk < CHIP_FREQ_FWS_5) {
                            EFC->EEFC_FMR = EEFC_FMR_FWS(5u) | EEFC_FMR_CLOE;
                        } else {
                            EFC->EEFC_FMR = EEFC_FMR_FWS(6u) | EEFC_FMR_CLOE;
                        }
                    }
                }
            }
        }
    }
}

void Clock_interface::initialize(Frequency) {
    setup_main_clock();
    setup_PLL_clock();
    setup_master_clock();
    setup_embedded_flash(calc_master_hz());
    setup_millisecond_timer();
    setup_microsecond_timer();
}

void Clock_interface::setup_main_clock() {
    // Enable Main Crystal Oscillator
    PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) | CKGR_MOR_MOSCXTEN |
                    CKGR_MOR_MOSCXTST(CHIP_FREQ_SLCK_RC);
    while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
    // Switch Main Clock to Main Crystal Oscillator clock
    PMC->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
    // Switch Master Clock to Main Clock
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | SYSCLK_PRES_1;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}

void Clock_interface::setup_PLL_clock() {
    uint32_t input_multiplier = 15u;
    uint32_t front_end_divider = 3u;
    PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0u);
    PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(input_multiplier) | CKGR_PLLAR_DIVA(front_end_divider) |
                      CKGR_PLLAR_PLLACOUNT(0x3Fu);
    while (!(PMC->PMC_SR & PMC_SR_LOCKA));
}

void Clock_interface::setup_master_clock() {
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_MDIV_Msk)) | PMC_MCKR_MDIV_PCK_DIV2;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | SYSCLK_PRES_2;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_PLLA_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}

void Clock_interface::setup_millisecond_timer() {
    const uint32_t frequency = enable_peripheral_clock(ID_TC0);
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_CPCTRG | TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK2;
    TC0->TC_CHANNEL[0].TC_RC = frequency / 1000 / 8;
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    TC0->TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN | TC_CCR_SWTRG);
    NVIC_EnableIRQ(ID_TC0);
}

void Clock_interface::setup_microsecond_timer() {
// TIMER_CLOCK2 = MCK/8
// TIMER_CLOCK3 = MCK/32
// TIMER_CLOCK4 = MCK/128
    enable_peripheral_clock(ID_TC7);
    enable_peripheral_clock(ID_TC8);
    TC2->TC_BMR = TC_BMR_TC2XC2S_TIOA1;
    TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR;
    TC2->TC_CHANNEL[1].TC_RA = 0xFFF0u;
    TC2->TC_CHANNEL[1].TC_RC = 0xFFFEu;
    TC2->TC_CHANNEL[2].TC_CMR = TC_CMR_WAVE | TC_CMR_TCCLKS_XC2;
    TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

uint32_t Clock_interface::get_microsecond_clock() {
    return Clock_interface::calc_master_hz() / 32;
}

uint64_t Clock_interface::get_microsecond_timestamp() {
    const auto *cv1 = &(TC2->TC_CHANNEL[1].TC_CV);
    const auto *cv2 = &(TC2->TC_CHANNEL[2].TC_CV);
    return (*cv1 | (*cv2 << 16u)) << 1;
}

uint32_t Clock_interface::get_hundredthsecond_timestamp() { return 0; }

uint32_t Clock_interface::get_peripheral_hz() {
    uint32_t peripheral_hz = calc_master_hz() / 2;
    return peripheral_hz;
}

uint32_t Clock_interface::enable_peripheral_clock(uint32_t peripheral_id) {
    uint32_t pcr;
    PMC->PMC_PCR = peripheral_id & 0x7Fu;
    pcr = PMC->PMC_PCR | PMC_PCR_EN | PMC_PCR_CMD;
    PMC->PMC_PCR = pcr;
    return get_peripheral_hz();
}

uint32_t Clock_interface::calc_master_hz() {
    uint32_t master_clock = 0;
    switch (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) {
        case PMC_MCKR_CSS_SLOW_CLK: // Slow clock
            if (SUPC->SUPC_SR & SUPC_SR_OSCSEL) {
                master_clock = CHIP_FREQ_XTAL_32K;
            } else {
                master_clock = CHIP_FREQ_SLCK_RC;
            }
            break;
        case PMC_MCKR_CSS_MAIN_CLK: // Main clock
            if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
                master_clock = CHIP_FREQ_XTAL_12M;
            } else {
                master_clock = CHIP_FREQ_MAINCK_RC_4MHZ;
                switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk) {
                    case CKGR_MOR_MOSCRCF_4_MHz:
                        break;
                    case CKGR_MOR_MOSCRCF_8_MHz:
                        master_clock *= 2U;
                        break;
                    case CKGR_MOR_MOSCRCF_12_MHz:
                        master_clock *= 3U;
                        break;
                    default:
                        break;
                }
            }
            break;
        case PMC_MCKR_CSS_PLLA_CLK: // PLLA clock
            if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
                master_clock = CHIP_FREQ_XTAL_12M;
            } else {
                master_clock = CHIP_FREQ_MAINCK_RC_4MHZ;
                switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk) {
                    case CKGR_MOR_MOSCRCF_4_MHz:
                        break;
                    case CKGR_MOR_MOSCRCF_8_MHz:
                        master_clock *= 2U;
                        break;
                    case CKGR_MOR_MOSCRCF_12_MHz:
                        master_clock *= 3U;
                        break;
                    default:
                        break;
                }
            }
            if ((uint32_t) (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLA_CLK) {
                master_clock *= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1U);
                master_clock /= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_DIVA_Msk) >> CKGR_PLLAR_DIVA_Pos));
            }
            break;
        default:
            break;
    }
    if ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) == PMC_MCKR_PRES_CLK_3) {
        master_clock /= 3U;
    } else {
        master_clock >>= ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) >> PMC_MCKR_PRES_Pos);
    }
    return master_clock;
}

void Clock_interface::report(Service_report &reporter) {
    reporter.begin_object(NORM_interface::Key_clock);
    reporter.report(NORM_interface::Key_frequency, get_master_clock());
    reporter.report(NORM_interface::Key_frequency, get_peripheral_hz());
    reporter.report(NORM_interface::Key_frequency, get_microsecond_clock());
    reporter.end_object();
}
