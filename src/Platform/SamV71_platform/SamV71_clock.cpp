/*
 * Copyright (C) 2024 Integrated Detector Electronics AS
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
 * \date   IDEAS/31.01.2020/aeols
 * \brief
 */

#include <cstdint>


#include "sam.h"
#include "component/pmc.h"
#include "SamV71_clock.h"

#include "Utility/Utility_types.h"

enum
{
    TC_DIV_FACTOR = 65536, //TC divisor used to find the lowest acceptable timer frequency
    FREQ_SLOW_CLOCK_EXT = 32768 // External slow clock frequency (hz)
};

Frequency Abstract_clock::the_master_Hz = 0;
Frequency Abstract_clock::the_PLLA_Hz = 0;
Frequency Abstract_clock::the_PLLB_Hz = 0;
volatile uint32_t Abstract_clock::channel_status = 0;
volatile uint32_t Abstract_clock::milliseconds_allmost_since_start = 0;
//volatile uint32_t Clock_interface::j2000_frac_counter = 0;
//volatile uint32_t Clock_interface::j2000_sec_counter = 0;
//volatile uint32_t Clock_interface::j2000_rc_counter = 0;
volatile uint32_t Abstract_clock::hundredthseconds_timestamp = 0;

uint32_t Abstract_clock::get_millisecond_timestamp() { return Abstract_clock::milliseconds_allmost_since_start; }

namespace Interrupt_service_routines
{
    __attribute__ ((section(".ISR"))) void ISR_timer0_ch0()
    {
        Abstract_clock::channel_status = TC0_REGS->TC_CHANNEL[0].TC_SR;
        Abstract_clock::milliseconds_allmost_since_start++;
    }

    __attribute__ ((section(".ISR"))) void ISR_timer0_ch1()
    {
        Abstract_clock::channel_status = TC0_REGS->TC_CHANNEL[1].TC_SR;
        Abstract_clock::hundredthseconds_timestamp++;
    }
}

void Abstract_clock::disable_PLLs()
{
    SUPC_REGS->SUPC_CR = SUPC_CR_KEY_PASSWD;
    // Enable the RC Oscillator
    PMC_REGS->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN_Msk;
    // Wait until the RC oscillator clock is ready.
    while ((PMC_REGS->PMC_SR & PMC_SR_MOSCRCS_Msk) != PMC_SR_MOSCRCS_Msk);
    // Configure the RC Oscillator frequency
    PMC_REGS->CKGR_MOR = (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) | CKGR_MOR_KEY_PASSWD;
    // Wait until the RC oscillator clock is ready
    while ((PMC_REGS->PMC_SR & PMC_SR_MOSCRCS_Msk) != PMC_SR_MOSCRCS_Msk);
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);
    // Set Master Clock Source to Slow Clock, MD_SLCK -> MCK
    PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_SLOW_CLK_Val;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);
    // Disable the PLLs
    PMC_REGS->CKGR_PLLAR = CKGR_PLLAR_ONE_Msk;
}

void Abstract_clock::initialize_main_clock()
{
    // Enable Main Crystal Oscillator
    PMC_REGS->CKGR_MOR = (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCXTST_Msk) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0u) |
        CKGR_MOR_MOSCXTEN_Msk;
    // Wait until the main oscillator clock is ready
    int max = 10000;
    while ((PMC_REGS->PMC_SR & PMC_SR_MOSCXTS_Msk) != PMC_SR_MOSCXTS_Msk)
    {
        if (0 > max--) break;
    }
    // Switch Main Clock to Main Crystal Oscillator clock
    PMC_REGS->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL_Msk;
    // Wait until MAINCK is switched to Main Crystal Oscillator
    max = 10000;
    while ((PMC_REGS->PMC_SR & PMC_SR_MOSCSELS_Msk) != PMC_SR_MOSCSELS_Msk)
    {
        if (0 > max--) break;
    }
}

Frequency Abstract_clock::initialize_PLLA(Frequency oscillator)
{
    volatile auto PMC_base = PMC_REGS;
    uint32_t front_end_divider;
    uint32_t multiplication_factor;
    if (oscillator == 20'000'000)
    {
        // DHU EM, PLLA: 64MHz = (20/5) * (15+1)
        front_end_divider = 5u;
        multiplication_factor = 15u;
    }
    else if (oscillator == 16'000'000)
    {
        // DHU EQM, PLLA: 64MHz = (16/1) * (3+1)
        front_end_divider = 1u;
        multiplication_factor = 3u;
    }
    else
    {
        // 10'000'000
        // RH71 EK, PLLA: 64MHz ~ (10/2) * (12+1) = 65MHz
        // 64 ~ (10/3) * (18+1) = 63.33MHz
        // NOTE: Max multiplier = 25
        front_end_divider = 2u;
        multiplication_factor = 12u;
    }
    PMC_base->CKGR_PLLAR = CKGR_PLLAR_ONE_Msk;
    PMC_base->CKGR_PLLAR = CKGR_PLLAR_ONE_Msk | // Must be set to one
        CKGR_PLLAR_PLLACOUNT(0x30u) |
        CKGR_PLLAR_MULA(multiplication_factor) |
        CKGR_PLLAR_DIVA(front_end_divider);
    while (!(PMC_base->PMC_SR & PMC_SR_LOCKA_Msk));
    return oscillator * (multiplication_factor + 1) / front_end_divider; // = 64'000'000;
}

Frequency Abstract_clock::initialize_PLLB(Frequency oscillator)
{
    volatile auto PMC_base = PMC_REGS;
    uint32_t front_end_divider;
    uint32_t multiplication_factor;
    if (oscillator == 20'000'000)
    {
        // DHU EM, PLLB: 60MHz = (20/4) * (11+1)
        front_end_divider = 4u;
        multiplication_factor = 11u;
    }
    else if (oscillator == 16'000'000)
    {
        // DHU EQM, PLLB: 60MHz = (16/4) * (14+1)
        front_end_divider = 4u;
        multiplication_factor = 14u;
    }
    else
    {
        // 10'000'000
        // RH71 EK, PLLB: 60MHz = (10/1) * (5+1)
        front_end_divider = 1u;
        multiplication_factor = 5u;
    }
    PMC_base->CKGR_PLLBR = 0;
    PMC_base->CKGR_PLLBR = CKGR_PLLBR_SRCB_MAINCK |
        CKGR_PLLBR_FREQ_VCO_VCO0 | // Frequency range: 40 – 80 MHz
        CKGR_PLLBR_PLLBCOUNT(0x30u) |
        CKGR_PLLBR_MULB(multiplication_factor) |
        CKGR_PLLBR_DIVB(front_end_divider);
    while (!(PMC_base->PMC_SR & PMC_SR_LOCKB_Msk));
    return oscillator * (multiplication_factor + 1) / front_end_divider; // = 60'000'000
}

Frequency Abstract_clock::initialize_master_clock(Frequency PLLA_Hz)
{
    // Prescaler divide by 2. Program PMC_MCKR.PRES and wait for PMC_SR.MCKRDY to be set
    PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_2;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);
    // Set Master Clock Division 1. Program PMC_MCKR.MDIV and Wait for PMC_SR.MCKRDY to be set
    PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_MDIV_Msk) | PMC_MCKR_MDIV_EQ_PCK;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);
    // Set Master Clock Source PLLA. Program PMC_MCKR.CSS and Wait for PMC_SR.MCKRDY to be set
    PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_PLLA_CLK;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);
    return PLLA_Hz / 2;
}

void Abstract_clock::initialize_clocks(Frequency oscillator)
{
    initialize_main_clock();
    the_PLLA_Hz = initialize_PLLA(oscillator);
    the_PLLB_Hz = initialize_PLLB(oscillator);
    the_master_Hz = initialize_master_clock(the_PLLA_Hz);
}

void Abstract_clock::initialize_timers()
{
    Abstract_clock::setup_millisecond_timer();
    Abstract_clock::setup_microsecond_timer();
    Abstract_clock::setup_hundredthsecond_timer();
}

void Abstract_clock::setup_millisecond_timer()
{
    volatile tc_registers_t* base = TC0_REGS;
    const uint32_t frequency = enable_peripheral_clock(ID_TC0_CHANNEL0);
    base->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVEFORM_WAVSEL_UP_RC | TC_CMR_WAVE_Msk;
    base->TC_CHANNEL[0].TC_RC = frequency / 1000;
    NVIC_EnableIRQ(TC0_CH0_IRQn);
    base->TC_CHANNEL[0].TC_IER = TC_IER_CPCS_Msk;
    milliseconds_allmost_since_start = 0;
    base->TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN_Msk | TC_CCR_SWTRG_Msk);
}

uint32_t Abstract_clock::get_channel_status()
{
    return channel_status;
}

uint32_t Abstract_clock::get_master_clock()
{
    return the_master_Hz;
}

uint32_t Abstract_clock::get_milliseconds()
{
    return milliseconds_allmost_since_start;
}

void Abstract_clock::setup_microsecond_timer()
{
    // TIMER_CLOCK2 = MCK/8
    // TIMER_CLOCK3 = MCK/32
    // TIMER_CLOCK4 = MCK/128
    volatile tc_registers_t* base = TC3_REGS;
    enable_peripheral_clock(ID_TC3_CHANNEL1);
    enable_peripheral_clock(ID_TC3_CHANNEL2);
    base->TC_BMR = TC_BMR_TC2XC2S_TIOA1;
    base->TC_CHANNEL[1].TC_CMR = TC_CMR_WAVEFORM_WAVSEL_UP | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVEFORM_ACPA_SET
        | TC_CMR_WAVEFORM_ACPC_CLEAR | TC_CMR_WAVE_Msk;
    base->TC_CHANNEL[1].TC_RA = 0xFFF0u;
    base->TC_CHANNEL[1].TC_RC = 0xFFFEu;
    base->TC_CHANNEL[2].TC_CMR = TC_CMR_WAVEFORM_WAVSEL_UP | TC_CMR_TCCLKS_XC2 | TC_CMR_WAVE_Msk;
    // Enable and start both timers
    base->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN(1u) | TC_CCR_SWTRG(1u);
    base->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN(1u) | TC_CCR_SWTRG(1u);
}

void Abstract_clock::setup_hundredthsecond_timer()
{
    volatile tc_registers_t* base = TC0_REGS;
    const uint32_t frequency = enable_peripheral_clock(ID_TC0_CHANNEL1);
    base->TC_CHANNEL[1].TC_CMR = TC_CMR_WAVEFORM_WAVSEL_UP_RC | TC_CMR_WAVE_Msk;
    base->TC_CHANNEL[1].TC_RC = (frequency / 100);
    NVIC_EnableIRQ(TC0_CH1_IRQn);
    base->TC_CHANNEL[1].TC_IER = TC_IER_CPCS_Msk;
    base->TC_CHANNEL[1].TC_CCR = (TC_CCR_CLKEN_Msk | TC_CCR_SWTRG_Msk);
}

uint32_t Abstract_clock::get_hundredthsecond_timestamp() { return Abstract_clock::hundredthseconds_timestamp; }

uint64_t Abstract_clock::get_microsecond_timestamp()
{
    volatile tc_registers_t* base = TC3_REGS;
    const auto* cv1 = &(base->TC_CHANNEL[1].TC_CV);
    const auto* cv2 = &(base->TC_CHANNEL[2].TC_CV);
    return (uint64_t)*cv1 | ((uint64_t)*cv2 << 32u);
}

uint32_t Abstract_clock::get_microsecond_clock()
{
    return Abstract_clock::the_master_Hz / 32;
}

uint32_t Abstract_clock::enable_peripheral_clock(uint32_t peripheral_id)
{
    // TODO: A clear division between using peripheral clock v.s. the generic clock.
    Frequency peripheral_Hz = 0;
    switch (peripheral_id)
    {
    case ID_ICM:
        peripheral_Hz = the_master_Hz; // MCK = PLLA(64MHz) / 2 = 32MHz
        PMC_REGS->PMC_PCR = PMC_PCR_EN_Msk | PMC_PCR_CMD_Msk | PMC_PCR_PID(peripheral_id);
        break;
    case ID_MCAN0:
    case ID_MCAN1:
        peripheral_Hz = Abstract_clock::the_PLLB_Hz / 1; // PLLB(60MHz) / (0+1) = 60MHz
        PMC_REGS->PMC_PCR = PMC_PCR_EN_Msk | PMC_PCR_CMD_Msk | PMC_PCR_PID(peripheral_id) |
            PMC_PCR_GCLKEN_Msk | PMC_PCR_GCLKDIV(0U);
        break;
    default:
        peripheral_Hz = Abstract_clock::the_PLLB_Hz / 6; // PLLB(60MHz) / (5+1) = 10MHz
        PMC_REGS->PMC_PCR = PMC_PCR_EN_Msk | PMC_PCR_CMD_Msk | PMC_PCR_PID(peripheral_id) |
            PMC_PCR_GCLKEN_Msk | PMC_PCR_GCLKDIV(5U);
        break;
    }
    return peripheral_Hz;
}
