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
* @file   IDE3380_readout_control.cpp
* @author AndersEmilOlsen, IDEAS
* @date   13.05.2026
* @brief  
*/

#include <cstdio>

#include "IDE3380_readout_control.h"
#include "IDE3380_interface.h"

// #include "spi.h"
// #include "stm32u575xx.h"
// #include "stm32u5xx_hal_tim.h"
// #include "tim.h"


// extern "C" void HAL_SPI_RxHalfCpltCallback(const SPI_HandleTypeDef *hspi) {
//     using namespace IDE3380;
//     if (hspi == &hspi2) {
//         if (IDE3380_readout_control::optional_readout_controller) {
//             uint16_t data =
//                     IDE3380_readout_control::data_rx_TXD[0] << 8 |
//                     IDE3380_readout_control::data_rx_TXD[1];
//             IDE3380_readout_control::optional_readout_controller->on_TXD_data(data);
//         }
//     }
// }

// extern "C" void HAL_SPI_RxCpltCallback(const SPI_HandleTypeDef *hspi) {
//     using namespace IDE3380;
//     if (hspi == &hspi2) {
//         uint16_t data =
//                 IDE3380_readout_control::data_rx_TXD[2] << 8 |
//                 IDE3380_readout_control::data_rx_TXD[3];
//         IDE3380_readout_control::optional_readout_controller->on_TXD_data(data);
//     }
// }

namespace IDE3380 {
    uint8_t IDE3380_readout_control::data_rx_TXD[4]{};
    void *IDE3380_readout_control::optional_IDE3380_user{};
    readout_func IDE3380_readout_control::optional_readout_func{};
    IDE3380_readout_control *IDE3380_readout_control::optional_readout_controller{};

    void IDE3380_readout_control::initialize(void *user, const readout_func readout) {
        optional_readout_controller = this;
        optional_IDE3380_user = user;
        optional_readout_func = readout;
        // HAL_SPI_Init(&hspi2);
        // HAL_SPI_Receive_DMA(&hspi2, data_rx_TXD, 4);
        // HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
        // TIM15->CCR1 = 0;
    }

    void IDE3380_readout_control::start_IDE3380_SYSCLK_I() {
        enum { CLK_CCR = 20, };
        //set the clock to 50% duty cycle at 4MHz in order for the IDE3380 to do the ADC conversion and send the data over the TXD pin
        // TIM15->CCR1 = CLK_CCR;
    }

    void IDE3380_readout_control::stop_IDE3380_SYSCLK_I() {
        // TIM15->CCR1 = 0;
    }

    void IDE3380_readout_control::on_TXD_data(const uint16_t irq_data) {
        cnt_TXD_data = cnt_TXD_data + 1;
        //parse the data from the TXD pin to get the ADC value and channel number.
        //The data is 22 bits long.
        //The data is sent in 16 bit chunks.
        //0 is returned if the data is not ready to be processed.
        uint32_t TXD_data = TXD_Parser(irq_data);
        if (TXD_data) {
            //parse the data into a struct
            TXD_data_t data{};
            data.trigger_flag = (TXD_data >> 20) & 0x001;
            data.ADC_Channel = (TXD_data >> 15) & 0x01F;
            data.trigger = (TXD_data >> 13) & 0x003;
            data.ADC_value = (TXD_data >> 1) & 0xFFF;
            if (IDE3380_readout_control::optional_readout_func) {
                optional_readout_func(optional_IDE3380_user, data);
            }
            if (data.ADC_Channel <= IDE3380_last_channel) {
                // HAL_GPIO_WritePin(IDE3380_DCAL_GPIO_Port, IDE3380_DCAL_Pin, GPIO_PIN_RESET);
            }
        }
        //if the number of TXD iterations is not 0. decrement the number of iterations
        //if the number of iterations is 0. stop the clock
        //this is done to allow the IDE3380 to do the ADC conversion and send the data over the TXD pin
        //if txd_iterations is 0. it means the IDE3380 has not requested any other data transfers
        decrement_iteration();
        if (is_TXD_idle()) {
            stop_IDE3380_SYSCLK_I();
        }
    }

    uint32_t IDE3380_readout_control::TXD_Parser(uint16_t data_in) {
        uint32_t return_value = 0;
        /*****************************************************************
        * If the data is not ready to be processed return 0
        * The data is ready to be processed if the first 16 bits are not 0
        * or the index is not 0
        *****************************************************************/
        if (the_bit_index == 0 && data_in == 0x0000) {
            return return_value; //no new data
        }
        /*****************************************************************
        * If the last data was started but not finished processing
        *****************************************************************/
        if (the_bit_index > 0) {
            //data already received fill the rest with the bits up to 22

            // restart_TXD_iteration();


            the_data_buffer |= (data_in << (22 - 16)) >> the_bit_index;
            the_bit_index += 16;
            if (the_bit_index >= 22) {
                //22 bits collected
                //process data
                return_value = the_data_buffer; //return data
                the_data_buffer = 0; //clear data
                data_in &= (0x0000FFFF >> (22 + 16 - the_bit_index)); //remove processed data
                the_bit_index = 0; //reset index
            } else return return_value; //not done collecting data
        }
        /*****************************************************************
         * Find the start of the data
         * if the data is not 0 find the MSB
         *****************************************************************/
        int8_t buff_start = 31 - __builtin_clz(data_in); //find the MSB
        if (buff_start >= 0) {
            //MSB found
            the_data_buffer = 0;
            the_data_buffer = data_in << (21 - buff_start);
            the_bit_index = buff_start + 1; //number of bits shifted
        }
        return return_value;
    }

    void IDE3380_readout_control::print_diag() const {
        printf("Readout: TXD=%d, TSUM_O=%d, events=%d, RxIter=%d, EXTI=%d, trigger=%s\r\n",
               cnt_TXD_data, cnt_TSUM_O, the_TORO_count, get_TXD_iteration(), the_EXTI_bits,
               is_ignore_trigger() ? "ignore" : "readout ");
    }

    void IDE3380_readout_control::on_GPIO_interrupt(uint16_t pin_mask) {
        the_EXTI_bits = pin_mask;
        // if (pin_mask & IDE3380_TOR_Pin) { count_TORO(); }
        // if (pin_mask & IDE3380_TSUM_Pin) { cnt_TSUM_O = cnt_TSUM_O + 1; }
        // if (pin_mask & (IDE3380_TSUM_Pin | IDE3380_TOR_Pin))
        {
            if (is_TXD_idle()) {
                if (!is_ignore_trigger()) {
                    restart_TXD_iteration();
                    start_IDE3380_SYSCLK_I();
                }
            }
        }
    }
} // IDE3380
