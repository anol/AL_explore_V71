/*
* Copyright (C) 2026 Integrated Detector Electronics AS
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
* @file   IDE3380_readout_control.h
* @author AndersEmilOlsen, IDEAS
* @date   13.05.2026
* @brief
*/

module;
#include <cstdint>

export module Component.IDE3380_readout_control;
export import Component.IDE3380_definitions;



export namespace IDE3380 {
    class IDE3380_readout_control {
        volatile uint32_t the_TORO_count{};
        volatile int the_TXD_iteration{};
        volatile bool the_inhibit_readout_flag{};
        volatile uint8_t the_bit_index{};
        volatile uint32_t the_data_buffer{};
        volatile uint32_t cnt_TXD_data{};
        volatile uint32_t cnt_TSUM_O{};
        volatile uint32_t cnt_double_O{};
        volatile uint32_t the_EXTI_bits{};

    public:
        static uint8_t data_rx_TXD[4];
        static void *optional_IDE3380_user;
        static readout_func optional_readout_func;
        static IDE3380_readout_control *optional_readout_controller;

        void initialize(void *user, readout_func readout);

        void start_IDE3380_SYSCLK_I();

        void stop_IDE3380_SYSCLK_I();

        void start_readout() {
            restart_TXD_iteration();
            start_IDE3380_SYSCLK_I();
        }

        bool continue_readout() {
            auto idle{is_TXD_idle()};
            if (idle) {
                stop_IDE3380_SYSCLK_I();
            }
            return idle;
        }

        void on_TXD_data(uint16_t irq_data);

        uint32_t TXD_Parser(uint16_t data_in);

        [[nodiscard]] bool is_TXD_idle() const {
            return the_TXD_iteration <= 0;
        }

        void decrement_iteration() {
            the_TXD_iteration = the_TXD_iteration - 1;
        }

        void restart_TXD_iteration() {
            the_TXD_iteration = IDE3380_readout_iterations;
        }

        [[nodiscard]] int get_TXD_iteration() const {
            return the_TXD_iteration;
        }

        void set_ignore_trigger(const bool flag) {
            the_inhibit_readout_flag = flag;
        }

        void print_diag() const;

        [[nodiscard]] bool is_ignore_trigger() const {
            return the_inhibit_readout_flag;
        }

        void on_GPIO_interrupt(uint16_t pin_mask);

        void count_TORO() {
            the_TORO_count = the_TORO_count + 1;
        }

        uint32_t reset_TORO_count() {
            auto events = the_TORO_count;
            the_TORO_count = 0;
            return events;
        };
    };
} // IDE3380
