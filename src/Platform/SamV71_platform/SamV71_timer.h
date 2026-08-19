//
// Created by anolsen on 31.01.2020.
//

#ifndef TEST_EVAL_RH71_TIMER_H
#define TEST_EVAL_RH71_TIMER_H

#include "Abstract_timer.h"
#include "component/tc.h"

class SamV71_timer : public Abstract_timer {
public:
    class Timer_definition {
    public:
        uint32_t peripheral_id;
        volatile tc_channel_registers_t *p_channel;
        Timer_function timer_function;
    };

private:
    Timer_definition the_definition;
    uint32_t the_clock_frequency{};

public:
    explicit SamV71_timer(const Timer_definition &definition);

    void initialize() override;

    bool enable_one_shot(Nanoseconds) override;

    bool enable_external_trigger(Nanoseconds, Optional_user, Optional_data,
                                 void (*)(Optional_user, Optional_data, bool success)) override;

    bool delayed_action(Nanoseconds, Optional_user, Optional_data,
        void (*func)(Optional_user, Optional_data, bool success)) override;

    bool start() override;

    bool stop() override;

    bool trigger() override;

    [[nodiscard]] uint32_t get_state() const override;

    void release_external_trigger() override;

    /* The functions below are only be available to the board_support (owner of the timers) */

    [[nodiscard]] volatile tc_channel_registers_t* get_channel() const {return the_definition.p_channel;}

    [[nodiscard]] volatile uint32_t get_peripheral_id() const {return the_definition.peripheral_id;}

    [[nodiscard]] volatile uint32_t get_timer_function() const {return the_definition.timer_function;}

    void reinitialize(Timer_function function);
};

#endif //TEST_EVAL_RH71_TIMER_H
