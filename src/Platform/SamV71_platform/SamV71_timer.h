//
// Created by anolsen on 31.01.2020.
//

#ifndef TEST_EVAL_RH71_TIMER_H
#define TEST_EVAL_RH71_TIMER_H

#include "Abstract_timer.h"
#include "component/tc.h"

class SamV71_timer : public Abstract_timer
{
public:
    class Timer_definition
    {
    public:
        uint32_t peripheral_id;
        tc_channel_registers_t* p_channel;
        Timer_function timer_function;
    };

private:
    Timer_definition the_definition;

public:
    explicit SamV71_timer(const Timer_definition& definition);

    void initialize() override;

    Status_code enable_one_shot(Nanoseconds) override;

    Status_code enable_external_trigger(Nanoseconds, Optional_user, Optional_data,
                                 void (*)(Optional_user, Optional_data, bool success)) override;

    Status_code delayed_action(Nanoseconds, Optional_user, Optional_data,
                        void (*func)(Optional_user, Optional_data, bool success)) override;

    Status_code start() override;

    Status_code stop() override;

    Status_code trigger() override;

    [[nodiscard]] uint32_t get_state() const override;

    void release_external_trigger() override;

    /* The functions below are only be available to the board_support (owner of the timers) */

    [[nodiscard]] tc_channel_registers_t* get_channel() const { return the_definition.p_channel; }

    [[nodiscard]] uint32_t get_peripheral_id() const { return the_definition.peripheral_id; }

    [[nodiscard]] uint32_t get_timer_function() const { return the_definition.timer_function; }

    void reinitialize(Timer_function function);
};

#endif //TEST_EVAL_RH71_TIMER_H
