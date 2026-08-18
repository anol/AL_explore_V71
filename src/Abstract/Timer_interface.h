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
 * \date   IDEAS/02.03.2020/aeols
 * \brief
 */

#ifndef NORM_FW_TIMER_INTERFACE_H
#define NORM_FW_TIMER_INTERFACE_H

#include <Utility/Utility_types.h>
#include <Dictionary/Dictionary.h>

class Service_report;

/// Purpose: Hardware abstraction of the special timer functions used to control the detector.
class Timer_interface {
public:
    enum Timer_function {
        MCU_milliseconds,           // ID_TC0_CHANNEL0
        MCU_hundredthseconds,       // ID_TC0_CHANNEL1
        MCU_NVM_NRESET_one_shot,    // ID_TC0_CHANNEL2
        DGU_DCAL_one_shot,          // ID_TC1_CHANNEL0
        DGU_HOLD_HG_rising_action,  // ID_TC1_CHANNEL1
        DGU_HOLD_HG_falling_action, // ID_TC1_CHANNEL1
        DGU_HOLD_LG_rising_action,  // ID_TC1_CHANNEL1
        DGU_THGV0_event_counter,    // ID_TC1_CHANNEL2
        NU_1_0_timer,               // ID_TC2_CHANNEL0
        NU_1_1_timer,               // ID_TC2_CHANNEL1
        DGU_bias_generator,         // ID_TC2_CHANNEL2
        NU_3_0_timer,               // ID_TC3_CHANNEL0
        MCU_microseconds_A,         // ID_TC3_CHANNEL1
        MCU_microseconds_B,         // ID_TC3_CHANNEL2
};

protected:
    Timer_function the_timer_function;
    Frequency the_frequency{};

public:
    explicit Timer_interface(Timer_function timer_function) : the_timer_function(timer_function) {}

    virtual void initialize() {};

    virtual bool enable_one_shot(Nanoseconds) { return true; }

    virtual bool enable_external_trigger(Nanoseconds, Optional_user, Optional_data,
                                         void (*)(Optional_user, Optional_data, bool success)) = 0;

    virtual bool delayed_action(Nanoseconds, Optional_user, Optional_data,
        void (*func)(Optional_user, Optional_data, bool success)) = 0;

    void set_frequency(Frequency frequency) { the_frequency = frequency; };

    Frequency get_frequency() const { return the_frequency; };

    virtual bool start() { return true; }

    virtual bool stop() { return true; }

    virtual bool trigger() { return true; }

    virtual void report_state(Service_report *, RMU_interface::Keys) const {}

    virtual uint32_t get_state() const = 0;

    virtual void release_external_trigger() = 0;
};

#endif //NORM_FW_TIMER_INTERFACE_H
