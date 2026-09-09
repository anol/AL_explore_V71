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
* @file   STM32U5_timer.cpp
* @author AndersEmilOlsen, IDEAS
* @date   16.03.2026
* @brief  
*/


#include "STM32U5_timer.h"
#include "main.h"
#include "Print_support.h"
#include "stm32u5xx_hal_rtc.h"
#include "System_clock.h"
#include "ux_api.h"

//timer interrupt
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM16) {
        // USB Poll Timer
        ux_device_stack_tasks_run();
    }
}

namespace STM32U575RG {
    void STM32U5_timer::initialize() {
        if (the_timer_function == Event_counter) {
            initialize_handle();
            configure_IO();
            configure_timer();
            the_state_mask &= ~Control_bit;
        }
    }

    uint32_t STM32U5_timer::get_count() const {
        uint32_t count{};
        if (the_state_mask == 0) {
            count = __HAL_TIM_GET_COUNTER(&the_handle);
        }
        return count;
    }

    uint32_t STM32U5_timer::reset_count() const {
        uint32_t count{};
        if (the_state_mask == 0) {
            count = __HAL_TIM_GET_COUNTER(&the_handle);
            __HAL_TIM_SET_COUNTER(&the_handle, 0);
        }
        return count;
    }

    void STM32U5_timer::print_diag() const {
        PRINTF("timer: func=%d, state=0x%X\r\n", the_timer_function, the_state_mask);
    }

    void STM32U5_timer::initialize_handle() {
        the_handle.Instance = TIM2;
        the_handle.Init.Prescaler = 0; // No prescaling
        the_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
        the_handle.Init.Period = 0xFFFFFFFF; // Max for 32‑bit timer
        the_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        the_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    }

    void STM32U5_timer::configure_IO() {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    void STM32U5_timer::configure_timer() {
        __HAL_RCC_TIM2_CLK_ENABLE();
        auto status = HAL_TIM_Base_Init(&the_handle);
        if (status == HAL_OK) the_state_mask &= ~Base_initialized;
        // Configure CH1 as input
        TIM_IC_InitTypeDef sIC{};
        sIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;  // or FALLING
        sIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
        sIC.ICPrescaler = TIM_ICPSC_DIV1;
        sIC.ICFilter    = 4;     // increase if signal is noisy
        status = HAL_TIM_IC_ConfigChannel(&the_handle, &sIC, TIM_CHANNEL_1);
        if (status == HAL_OK) the_state_mask &= ~Counter_initialized;
        // Configure External Clock Mode 1
        TIM_ClockConfigTypeDef sClock = {0};
        sClock.ClockSource     = TIM_CLOCKSOURCE_TI1;
        sClock.ClockPolarity   = TIM_CLOCKPOLARITY_NONINVERTED;
        sClock.ClockPrescaler  = TIM_CLOCKPRESCALER_DIV1;
        sClock.ClockFilter     = 0;
        status = HAL_TIM_ConfigClockSource(&the_handle, &sClock);
        if (status == HAL_OK) the_state_mask &= ~Source_initialized;
        // Start timer
        status = HAL_TIM_Base_Start(&the_handle);
        if (status == HAL_OK) the_state_mask &= ~Timer_started;
    }
} // Application
