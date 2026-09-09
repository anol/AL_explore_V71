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
* @file   Cadence_control.cpp
* @author AndersEmilOlsen, IDEAS
* @date   25.03.2026
* @brief  
*/


#include "Cadence_control.h"
// #include "rtc.h"
// #include "System_clock.h"

// extern "C" void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef */*hrtc*/) {
//     // HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, static_cast<GPIO_PinState>(0));
//     if (auto *control = Application::Cadence_control::get_cadence_control()) {
//         control->ISR_on_wakeup();
//     }
//     // HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, static_cast<GPIO_PinState>(1));
// }

namespace Application {
    Cadence_control *Cadence_control::optional_the_one_and_only{};

    void Cadence_control::initialize() {
        optional_the_one_and_only = this;
        // HAL_RTCEx_DeactivateWakeUpTimer(&hrtc); // Cadence Timer
    }

    void Cadence_control::ISR_on_wakeup() {
        cnt_wakeup = cnt_wakeup + 1;
        if (cnt_wakeup >= the_cadence) {
            // --- final wake-up: leave sleep-on-exit off, go back to run mode ---
            cnt_wakeup = 0;
            // HAL_PWR_DisableSleepOnExit();
            // bring clocks & tick back
            // SystemClock_Config();
            // HAL_ResumeTick();
            if (optional_func) {
                optional_func(optional_user);
            }
            ISR_time_to_send();
            ISR_calibration_time();
        }
    }

    bool Cadence_control::reset_calibration_time() {
        // uint32_t primask_bit = __get_PRIMASK();
        // __disable_irq();
        auto flag = the_calibration_flag;
        the_calibration_flag = false;
        // __set_PRIMASK(primask_bit);
        return flag;
    }

    bool Cadence_control::reset_time_to_send() {
        // uint32_t primask_bit = __get_PRIMASK();
        // __disable_irq();
        auto flag = the_telemetry_flag;
        // the_telemetry_flag = false;
        // __set_PRIMASK(primask_bit);
        return flag;
    }

    void Cadence_control::set_next_wakeup() const {
        // enum {
        //     Wakeup_counter = 2048, // Wakeup in 2 seconds
        // };
        // if (the_cadence > 0) {
        //     HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2048, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0);
        // } else {
        //     HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
        // }
    }
} // Application
