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
* @file   STM32U5_timer.h
* @author AndersEmilOlsen, IDEAS
* @date   16.03.2026
* @brief  
*/

#pragma once

#include "tim.h"

namespace STM32U575RG {
    class STM32U5_timer {
        enum : uint32_t {
            Base_initialized = 0b1000'0000,
            Counter_initialized = 0b0100'0000,
            Source_initialized = 0b0010'0000,
            Timer_started = 0b0001'0000,
            Control_bit = 0b0000'0001,
            Initial_state_mask =
            Base_initialized | Counter_initialized | Source_initialized | Timer_started | Control_bit,
        };

    public:
        enum Timer_function { Not_function, Event_counter };

    private:
        const Timer_function the_timer_function;
        uint32_t the_state_mask{Initial_state_mask};
        TIM_HandleTypeDef the_handle{};

    public:
        explicit STM32U5_timer(const Timer_function func) : the_timer_function(func) {
        };

        void initialize();

        [[nodiscard]] uint32_t get_count() const;

        [[nodiscard]] uint32_t reset_count() const;

        void print_diag() const;

    private:
        void initialize_handle();

        static void configure_IO();

        void configure_timer();
    };
} // Application
