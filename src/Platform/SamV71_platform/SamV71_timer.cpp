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

//
// Created by anolsen on 31.01.2020.
//

#include <stdio.h>

#include "Utility/Unit_converter.h"
#include "Dictionary/Dictionary.h"
#include "sam.h"
#include "SamV71_clock.h"
#include "SamV71/component/interrupt_controller/nvic_helpers.h"

#include "SamV71_timer.h"

#include <SamV71_watchdog.h>

using namespace Unit_converter;

constexpr uint32_t Software_only_channel_mode =
        TC_CMR_WAVE_Msk // Waveform mode
        | TC_CMR_WAVEFORM_WAVSEL_UP_RC // Increment from 0 to RC
        | TC_CMR_WAVEFORM_CPCDIS_Msk // Disable counter clock at RC
        | TC_CMR_WAVEFORM_CPCSTOP_Msk // Stop counter clock at RC
        | TC_CMR_TCCLKS_TIMER_CLOCK1; // Use the GCLK clock

constexpr uint32_t Software_trigger_channel_mode =
        TC_CMR_WAVE_Msk // Waveform mode
        | TC_CMR_WAVEFORM_WAVSEL_UP_RC // Increment from 0 to RC
        | TC_CMR_WAVEFORM_ASWTRG_SET // Set TIOAx on software trigger (SWTRG)
        | TC_CMR_WAVEFORM_ACPC_CLEAR // Clear TIOAx on RC compare
        | TC_CMR_WAVEFORM_CPCDIS_Msk // Disable counter clock at RC
        | TC_CMR_WAVEFORM_CPCSTOP_Msk // Stop counter clock at RC
        | TC_CMR_TCCLKS_TIMER_CLOCK1; // Use the GCLK clock

constexpr uint32_t External_trigger_HG_falling_channel_mode =
        TC_CMR_WAVE_Msk // Waveform mode
        | TC_CMR_WAVEFORM_EEVT_TIOB // Select TIOB as external event (HGTVO)
        | TC_CMR_WAVEFORM_ENETRG_Msk // Reset and start on external event
        | TC_CMR_WAVEFORM_WAVSEL_UP_RC // Increment from 0 to RC
        | TC_CMR_WAVEFORM_EEVTEDG_FALLING // Trigger on falling edge
        | TC_CMR_WAVEFORM_ACPC_SET // Set TIOAx on RC compare
        | TC_CMR_WAVEFORM_ASWTRG_CLEAR // Clear TIOAx on software trigger (SWTRG)
        | TC_CMR_WAVEFORM_CPCDIS_Msk // Disable counter clock at RC
        | TC_CMR_WAVEFORM_CPCSTOP_Msk // Stop counter clock at RC
        | TC_CMR_TCCLKS_TIMER_CLOCK1; // Use the GCLK clock;

constexpr uint32_t External_trigger_HG_rising_channel_mode =
        TC_CMR_WAVE_Msk // Waveform mode
        | TC_CMR_WAVEFORM_EEVT_TIOB // Select TIOB as external event (HGTVO)
        | TC_CMR_WAVEFORM_ENETRG_Msk // Reset and start on external event
        | TC_CMR_WAVEFORM_WAVSEL_UP_RC // Increment from 0 to RC
        | TC_CMR_WAVEFORM_EEVTEDG_RISING // Trigger on rising edge
        | TC_CMR_WAVEFORM_ACPC_SET // Set TIOAx on RC compare
        | TC_CMR_WAVEFORM_ASWTRG_CLEAR // Clear TIOAx on software trigger (SWTRG)
        | TC_CMR_WAVEFORM_CPCDIS_Msk // Disable counter clock at RC
        | TC_CMR_WAVEFORM_CPCSTOP_Msk // Stop counter clock at RC
        | TC_CMR_TCCLKS_TIMER_CLOCK1; // Use the GCLK clock;

constexpr uint32_t External_trigger_LG_rising_channel_mode =
        TC_CMR_WAVE_Msk // Waveform mode
        | TC_CMR_WAVEFORM_EEVT_XC1 // Select XCx (TCLKx) as external event (LGTVO)
        | TC_CMR_WAVEFORM_ENETRG_Msk // Reset and start on external event
        | TC_CMR_WAVEFORM_WAVSEL_UP_RC // Increment from 0 to RC
        | TC_CMR_WAVEFORM_EEVTEDG_RISING // Trigger on rising edge
        | TC_CMR_WAVEFORM_ACPC_SET // Set TIOAx on RC compare
        | TC_CMR_WAVEFORM_ASWTRG_CLEAR // Clear TIOAx on software trigger (SWTRG)
        | TC_CMR_WAVEFORM_CPCDIS_Msk // Disable counter clock at RC
        | TC_CMR_WAVEFORM_CPCSTOP_Msk // Stop counter clock at RC
        | TC_CMR_TCCLKS_TIMER_CLOCK1; // Use the GCLK clock;

//Note: If TIOB is chosen as the external event signal, it is configured as an input and no longer generates waveforms
//and subsequently no IRQs.

constexpr uint32_t External_trigger_counter_mode =
        TC_CMR_WAVE_Msk // Waveform mode
        | TC_CMR_WAVEFORM_EEVT_XC1 // Select TIOB as external event
//        | TC_CMR_WAVEFORM_ENETRG_Msk // Reset and start on external event
        | TC_CMR_WAVEFORM_WAVSEL_UP // Increment, no automatic trigger
        | TC_CMR_WAVEFORM_ACPC_NONE // No action on TIOAx on RC compare
        | TC_CMR_WAVEFORM_ASWTRG_NONE // No action on TIOAx on software trigger (SWTRG)
        | TC_CMR_WAVEFORM_EEVTEDG_RISING
//        | TC_CMR_WAVEFORM_CPCDIS_Msk // Disable counter clock at RC
//        | TC_CMR_WAVEFORM_CPCSTOP_Msk // Stop counter clock at RC
        | TC_CMR_TCCLKS_XC1; // Use the GCLK clock;

constexpr uint32_t External_frequency_generator_mode =
        TC_CMR_WAVE_Msk // Waveform mode
        | TC_CMR_WAVEFORM_WAVSEL_UP_RC // Increment from 0 to RC
        | TC_CMR_WAVEFORM_ACPA_SET // Raise TIOAx on RA compare
        | TC_CMR_WAVEFORM_ACPC_CLEAR // Clear TIOAx on RC compare
        | TC_CMR_TCCLKS_TIMER_CLOCK1; // Use the GCLK clock

//TODO: TC1XC1S should be configured explicitly, even if reset value is correct
constexpr uint32_t Timer_input_mux_selection =
        TC_BMR_TC1XC1S_TCLK1;

static void (*TC0_CH2_callback)(void *, void *, bool) = nullptr;

static void *TC0_CH2_user = nullptr;
static void *TC0_CH2_data = nullptr;

static void set_TC0_CH2_callback(void *user, void *data, void (*func)(void *, void *, bool)) {
    TC0_CH2_callback = func;
    TC0_CH2_user = user;
    TC0_CH2_data = data;
}

namespace Interrupt_service_routines {
    void ISR_timer0_ch2() {
        auto *base{TC0_REGS};
        auto channel{2};
        uint32_t channel_status = base->TC_CHANNEL[channel].TC_SR;
        if (channel_status & TC_SR_CPCS_Msk) {
            // RC-Compare
            NVIC_DisableIRQ(TC1_CH1_IRQn);
            base->TC_CHANNEL[channel].TC_IDR = TC_IDR_CPCS_Msk;
            base->TC_CHANNEL[channel].TC_RC = 0xFFFFFFFF;
            if (TC0_CH2_callback != nullptr) {
                auto user = TC0_CH2_user;
                auto data = TC0_CH2_data;
                auto callback = TC0_CH2_callback;
                TC0_CH2_callback = nullptr;
                callback(user, data, true);
            }
        }
    }
}

static void (*TC1_CH1_callback)(void *, void *, bool) = nullptr;

static void *TC1_CH1_user = nullptr;
static void *TC1_CH1_data = nullptr;

static void set_TC1_CH1_callback(void *user, void *data, void (*func)(void *, void *, bool)) {
    TC1_CH1_callback = func;
    TC1_CH1_user = user;
    TC1_CH1_data = data;
}

namespace Interrupt_service_routines {

    __attribute__ ((section(".ISR"))) void ISR_timer1_ch1() {
        // TC1 CH1 is used for the THGVO to SS_HOLD timer, i.e. External_trigger_channel_mode
        // 1. Raising edge on THGVO will cause an external event, that will reset the counter and start the clock
        // 2. The same external event will cause an interrupt that we use asap to disable it from doing further resets
        // 3. The SS_HOLD will be set when the counter reaches RC
        // 4. The SS_HOLD will be cleared and the timer will be disabled the timer is released by software
        uint32_t channel_status = TC1_REGS->TC_CHANNEL[1].TC_SR;
        if (channel_status & TC_SR_ETRGS_Msk) {
            // External event trigger
            TC1_REGS->TC_CHANNEL[1].TC_CMR &= ~TC_CMR_WAVEFORM_ENETRG_Msk; // Disable the external trigger.
            TC1_REGS->TC_CHANNEL[1].TC_IDR = TC_IDR_ETRGS_Msk;
        }
        if (channel_status & TC_SR_CPCS_Msk) {
            // RC-Compare
            NVIC_DisableIRQ(TC1_CH1_IRQn);
            TC1_REGS->TC_CHANNEL[1].TC_IDR = TC_IDR_CPCS_Msk;
            TC1_REGS->TC_CHANNEL[1].TC_RC = 0xFFFFFFFF;
            if (TC1_CH1_callback != nullptr) {
                auto user = TC1_CH1_user;
                auto data = TC1_CH1_data;
                auto callback = TC1_CH1_callback;
                TC1_CH1_callback = nullptr;
                callback(user, data, true);
            }
        }
    }
}

SamV71_timer::SamV71_timer(const Timer_definition &definition) : Timer_interface(definition.timer_function),
                                                                   the_definition(definition) {
}

void SamV71_timer::initialize() {
    the_clock_frequency = Clock_interface::enable_peripheral_clock(the_definition.peripheral_id);
    auto p_channel = the_definition.p_channel;
    p_channel->TC_CCR = TC_CCR_CLKDIS_Msk; // Disable clock
    p_channel->TC_IDR = 0xFFFFFFFF; // Disable interrupts
    switch (the_timer_function) {
        case MCU_NVM_NRESET_one_shot:
            p_channel->TC_CMR = Software_only_channel_mode;
            break;
        case DGU_DCAL_one_shot:
            p_channel->TC_CMR = Software_trigger_channel_mode;
            break;
        case DGU_HOLD_HG_falling_action:
            p_channel->TC_CMR = External_trigger_HG_falling_channel_mode;
            break;
        case DGU_HOLD_HG_rising_action:
            p_channel->TC_CMR = External_trigger_HG_rising_channel_mode;
            break;
        case DGU_HOLD_LG_rising_action:
            p_channel->TC_CMR = External_trigger_LG_rising_channel_mode;
            break;
        case DGU_bias_generator:
            p_channel->TC_CMR = External_frequency_generator_mode;
            break;
        case DGU_THGV0_event_counter:
            p_channel->TC_CMR = External_trigger_counter_mode;
            break;
        default:
            p_channel->TC_CMR = 0;
            break;
    }
}

void SamV71_timer::reinitialize(Timer_interface::Timer_function function)
{
    the_clock_frequency = Clock_interface::enable_peripheral_clock(the_definition.peripheral_id);
    auto p_channel = the_definition.p_channel;
    p_channel->TC_CCR = TC_CCR_CLKDIS_Msk; // Disable clock
    p_channel->TC_IDR = 0xFFFFFFFF; // Disable interrupts
    p_channel->TC_CMR = 0;

    the_timer_function = function;

    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
    SamV71_timer::initialize();
    __asm volatile ("dsb"); // Data Synchronization Barrier
    __asm volatile ("isb"); // Instruction Synchronization Barrier
}

bool SamV71_timer::trigger() {
    auto p_channel = the_definition.p_channel;
    p_channel->TC_CCR = TC_CCR_SWTRG_Msk;
    return true;
}

bool SamV71_timer::enable_one_shot(Nanoseconds pulse_width_ns) {
    auto p_channel = the_definition.p_channel;
    p_channel->TC_RC = (pulse_width_ns / 1000) * (the_clock_frequency / 1000000);
    p_channel->TC_CCR = TC_CCR_CLKEN_Msk;
    return true;
}

bool
SamV71_timer::enable_external_trigger(Nanoseconds pulse_delay_ns, Optional_user user, Optional_data data,
                                       void (*func)(Optional_user, Optional_data, bool success)) {
    NVIC_DisableIRQ(the_definition.peripheral_id);
    if (TC1_CH1_IRQn == the_definition.peripheral_id) {
        set_TC1_CH1_callback(user, data, func);
    }
    auto p_channel = the_definition.p_channel;
    if (p_channel != nullptr) {
        switch (the_timer_function)
        {
            case DGU_HOLD_HG_falling_action:
                p_channel->TC_CMR = External_trigger_HG_falling_channel_mode;
                break;
            case DGU_HOLD_HG_rising_action:
                p_channel->TC_CMR = External_trigger_HG_rising_channel_mode;
                break;
            case DGU_HOLD_LG_rising_action:
                p_channel->TC_CMR = External_trigger_LG_rising_channel_mode;
                break;
            default:
                p_channel->TC_CMR = 0;
                return false;
        }
        p_channel->TC_RC = nanoseconds_to_count(the_clock_frequency, pulse_delay_ns);
        p_channel->TC_CCR = TC_CCR_CLKEN_Msk; // Enable counter clock
        p_channel->TC_IER = TC_IER_CPCS_Msk | TC_IER_ETRGS_Msk; // Enable RC compare and external
    }
    NVIC_ClearPendingIRQ(the_definition.peripheral_id);
    NVIC_EnableIRQ(the_definition.peripheral_id);
    return true;
}

bool SamV71_timer::delayed_action(Nanoseconds pulse_delay_ns, Optional_user user, Optional_data data,
                                   void (*func)(Optional_user, Optional_data, bool success)) {
    NVIC_DisableIRQ(the_definition.peripheral_id);
    if (TC0_CH2_IRQn == the_definition.peripheral_id) {
        set_TC0_CH2_callback(user, data, func);
    }
    auto *p_channel = the_definition.p_channel;
    if (p_channel != nullptr) {
        p_channel->TC_RC = nanoseconds_to_count(the_clock_frequency, pulse_delay_ns);
        p_channel->TC_CCR = TC_CCR_CLKEN_Msk;
        p_channel->TC_IER = TC_IER_CPCS_Msk; // Enable RC compare
        p_channel->TC_CCR = TC_CCR_SWTRG_Msk;
    }
    NVIC_ClearPendingIRQ(the_definition.peripheral_id);
    NVIC_EnableIRQ(the_definition.peripheral_id);
    return true;
}

void SamV71_timer::release_external_trigger() {
    auto p_channel = the_definition.p_channel;
    if (p_channel != nullptr) {
        if (the_definition.p_channel->TC_IMR != 0) {
            NVIC_DisableIRQ(the_definition.peripheral_id);
            p_channel->TC_CCR = TC_CCR_CLKDIS_Msk;
            p_channel->TC_IDR = TC_IDR_CPCS_Msk | TC_IDR_ETRGS_Msk;
            //            p_channel->TC_CMR &= ~TC_CMR_WAVEFORM_ENETRG_Msk;
            p_channel->TC_RC = 0xFFFF'FFFF;
        }
        p_channel->TC_CCR = TC_CCR_SWTRG_Msk;
    }
    if (TC1_CH1_callback != nullptr) {
        auto user = TC1_CH1_user;
        TC1_CH1_user = nullptr;
        auto data = TC1_CH1_data;
        TC1_CH1_data = nullptr;
        auto callback = TC1_CH1_callback;
        TC1_CH1_callback = nullptr;
        callback(user, data, false);
    }
}

bool SamV71_timer::start() {
    auto p_channel = the_definition.p_channel;
    bool success = the_frequency > 0;
    if (success) {
        uint32_t count = the_clock_frequency / the_frequency;
        p_channel->TC_RA = count / 2;
        p_channel->TC_RC = count;
        p_channel->TC_CCR = TC_CCR_CLKEN_Msk;
        p_channel->TC_CCR = TC_CCR_SWTRG_Msk;
    }
    return success;
}

bool SamV71_timer::stop() {
    auto p_channel = the_definition.p_channel;
    p_channel->TC_CCR = TC_CCR_CLKDIS_Msk;
    return true;
}

void SamV71_timer::report_state(Service_report *reporter, RMU_interface::Keys k) const {
    if (reporter) {
        if (RMU_interface::No_key != k)
        {
            reporter->begin_object(k);
        }
        else
        {
            reporter->begin_object();
        }
        reporter->report(RMU_interface::Key_peripheral, the_definition.peripheral_id);
        reporter->report(RMU_interface::Key_function, (uint32_t) the_definition.timer_function);
        if (the_definition.p_channel != nullptr) {
            reporter->report(RMU_interface::Key_state, the_definition.p_channel->TC_IMR);
            reporter->report(RMU_interface::Key_count, the_definition.p_channel->TC_CV);
            reporter->report(RMU_interface::Key_load, the_definition.p_channel->TC_RC);
        }
        if (the_frequency > 0) {
            reporter->report(RMU_interface::Key_frequency, the_frequency);
        }
        reporter->end_object();
    }
}

uint32_t SamV71_timer::get_state() const {
    if (the_definition.p_channel == nullptr) return 0;
    return the_definition.p_channel->TC_IMR;
}
