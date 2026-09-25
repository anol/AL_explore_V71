module;

module Support.Heartbeat_task;

namespace Support {
    void Heartbeat_task::initialize() {
        use_pin_A.set();
        use_pin_B.set();
    }

    void Heartbeat_task::task_loop() {
        enum { Delay_millis = 1000 };
        while (true) {
            delay_until(Delay_millis);
            toggle_LED();
        }
    }

    void Heartbeat_task::toggle_LED() {
        the_toggle_flag = !the_toggle_flag;
        if (the_toggle_flag) {
            use_pin_A.set();
            use_pin_B.clear();
        } else {
            use_pin_A.clear();
            use_pin_B.set();
        }
    }
} // Support
