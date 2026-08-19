#pragma once

#include <Utility/Utility_types.h>

class Service_report;

/// Purpose: Hardware abstraction of the special timer functions used to control the detector.
class Abstract_timer
{
public:
    enum Timer_function
    {
        MCU_milliseconds, // ID_TC0_CHANNEL0
        MCU_hundredthseconds, // ID_TC0_CHANNEL1
        MCU_NVM_NRESET_one_shot, // ID_TC0_CHANNEL2
        DGU_DCAL_one_shot, // ID_TC1_CHANNEL0
        DGU_HOLD_HG_rising_action, // ID_TC1_CHANNEL1
        DGU_HOLD_HG_falling_action, // ID_TC1_CHANNEL1
        DGU_HOLD_LG_rising_action, // ID_TC1_CHANNEL1
        DGU_THGV0_event_counter, // ID_TC1_CHANNEL2
        NU_1_0_timer, // ID_TC2_CHANNEL0
        NU_1_1_timer, // ID_TC2_CHANNEL1
        DGU_bias_generator, // ID_TC2_CHANNEL2
        NU_3_0_timer, // ID_TC3_CHANNEL0
        MCU_microseconds_A, // ID_TC3_CHANNEL1
        MCU_microseconds_B, // ID_TC3_CHANNEL2
    };

protected:
    Timer_function the_timer_function;
    Frequency the_frequency{};

public:
    explicit Abstract_timer(Timer_function timer_function) : the_timer_function(timer_function)
    {
    }

    virtual ~Abstract_timer() = default;

    virtual void initialize()
    {
    };

    virtual bool enable_one_shot(Nanoseconds) { return true; }

    virtual bool enable_external_trigger(Nanoseconds, Optional_user, Optional_data,
                                         void (*)(Optional_user, Optional_data, bool success)) = 0;

    virtual bool delayed_action(Nanoseconds, Optional_user, Optional_data,
                                void (*func)(Optional_user, Optional_data, bool success)) = 0;

    void set_frequency(Frequency frequency) { the_frequency = frequency; };

    [[nodiscard]] Frequency get_frequency() const { return the_frequency; };

    virtual bool start() { return true; }

    virtual bool stop() { return true; }

    virtual bool trigger() { return true; }

    [[nodiscard]] virtual uint32_t get_state() const = 0;

    virtual void release_external_trigger() = 0;
};
