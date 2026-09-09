/*
* Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
* @file   Cadence_control.h
* @author AndersEmilOlsen, IDEAS
* @date   25.03.2026
* @brief  
*/


#pragma once
#include <cstdint>

namespace Application {
    using callback_t = void (*)(void *);

    class Cadence_control {
        static Cadence_control *optional_the_one_and_only;
        void *optional_user{};
        callback_t optional_func{};
        volatile uint32_t the_cadence{};
        volatile uint32_t cnt_wakeup{};
        volatile bool the_calibration_flag{};
        volatile bool the_telemetry_flag{};

    public:
        static Cadence_control *get_cadence_control() { return optional_the_one_and_only; }

        void initialize();

        void ISR_on_wakeup();

        void set_cadence_callback(void *user, const callback_t func) {
            optional_user = user;
            optional_func = func;
        }

        void set_cadence(const uint32_t cadence) { the_cadence = cadence; }

        [[nodiscard]] uint32_t get_cadence() const { return the_cadence; }

        void ISR_calibration_time() {
            the_calibration_flag = true;
        }

        [[nodiscard]] bool reset_calibration_time();

        void ISR_time_to_send() {
            the_telemetry_flag = true;
        }

        [[nodiscard]] bool reset_time_to_send();

        void set_next_wakeup() const;
    };
} // Application
