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
* @file   IDE3380_register_access.cpp
* @author AndersEmilOlsen, IDEAS
* @date   13.05.2026
* @brief  
*/

#include <stdio.h>

#include "IDE3380_register_access.h"

#include "IDE3380_register_decoder.h"
// #include "main.h"
// #include "spi.h"

namespace IDE3380 {
    IDE3380_register_access *IDE3380_register_access::optional_one_and_only{};
}

// extern "C" void HAL_SPI_TxRxCpltCallback(const SPI_HandleTypeDef *hspi) {
//     using namespace IDE3380;
//     // if (hspi == &hspi1) {
//     //     if (IDE3380_register_access::optional_one_and_only) {
//     //         IDE3380_register_access::optional_one_and_only->transfer_complete();
//     //     }
//     // }
// }

namespace IDE3380 {
    void IDE3380_register_access::initialize() {
        optional_one_and_only = this;
        // HAL_GPIO_WritePin(IDE3380_RST_GPIO_Port, IDE3380_RST_Pin, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(IDE3380_RST_GPIO_Port, IDE3380_RST_Pin, GPIO_PIN_RESET);
    }


    void IDE3380_register_access::dump() {
        for (uint8_t address = 0; address < IDE3380_register_count; address++) {
            auto value = SPI_read_register(address);
            printf("0x%02X=0x%08X\r\n", address, value);
        }
    }

    void IDE3380_register_access::print_diag() {
        uint32_t value[IDE3380_register_count]{};
        for (uint8_t address = 0; address < IDE3380_register_count; address++) {
            value[address] = SPI_read_register(address);
        }
        IDE3380_register_decoder::dump_decoded(value, IDE3380_register_count);
    }

    void IDE3380_register_access::override_all_thresholds() {
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            the_channel_restore_cache[index] = SPI_read_register(index);
        }
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            auto new_value = the_channel_restore_cache[index];
            new_value |= QC_threshold_mask; // Set max threshold
            SPI_update_register(index, IDE3380_write_only, new_value);
        }
    }

    void IDE3380_register_access::disable_all_channels() {
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            the_channel_restore_cache[index] = SPI_read_register(index);
        }
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            disable_channel(index);
        }
    }

    void IDE3380_register_access::restore_all_channels() {
        for (uint8_t index = 0; index < IDE3380_channel_count; index++) {
            SPI_update_register(index, IDE3380_write_only, the_channel_restore_cache[index]);
        }
    }

    void IDE3380_register_access::disable_channel(uint8_t index) {
        if (index < IDE3380_channel_count) {
            SPI_update_register(index, IDE3380_write_only, Disable_channel_setting);
        }
    }

    void IDE3380_register_access::enable_channel(const uint8_t index) {
        if (index < IDE3380_channel_count) {
            SPI_update_register(index, IDE3380_write_only, Enable_channel_and_max_threshold);
        }
    }

    uint32_t IDE3380_register_access::set_channel_threshold(const uint8_t index, const uint8_t threshold) {
        uint32_t value{};
        if (index < IDE3380_channel_count) {
            value = SPI_read_register(index);
            value &= ~QC_threshold_mask; // Clear old threshold
            value |= threshold << QC_threshold_pos; // Set new threshold
            SPI_update_register(index, IDE3380_write_only, value);
        }
        return value;
    }

    uint32_t IDE3380_register_access::SPI_update_register(uint8_t register_address, uint8_t nRead_Write,
                                                          uint32_t data) {
        if (register_address >= IDE3380_register_count)return 0;
        // uint32_t valid_mask = ~((~0) << IDE3380_register_width[register_address]);
        // uint32_t data_masked = data & valid_mask;
        // uint8_t data_rx[5];
        // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_SET);
        if (nRead_Write) {
            // uint8_t data_tx[5] = {0};
            // data_tx[0] = (register_address << 1) | 0x01; //write
            // data_tx[1] = data_masked >> 24 & 0xFF;
            // data_tx[2] = data_masked >> 16 & 0xFF;
            // data_tx[3] = data_masked >> 8 & 0xFF;
            // data_tx[4] = data_masked & 0xFF;
            // // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_RESET);
            // the_transfer_complete_flag = false;
            // // HAL_SPI_TransmitReceive_DMA(&hspi1, data_tx, data_rx, 5);
            // while (!the_transfer_complete_flag) {
            //     // HAL_Delay(1);
            // }
            // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_SET);
        }
        if (nRead_Write != IDE3380_write_only) {
            // uint8_t data_tx[5] = {0};
            // data_tx[0] = (register_address << 1) | 0x00; //read
            // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_RESET);
            // the_transfer_complete_flag = false;
            // HAL_SPI_TransmitReceive_DMA(&hspi1, data_tx, data_rx, 5);
            // while (!the_transfer_complete_flag) {
            //     HAL_Delay(1);
            // }
            // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_SET);
            // return (data_rx[1] << 24 | data_rx[2] << 16 | data_rx[3] << 8 | data_rx[4]) & valid_mask;
        }
        return 0;
    }

    uint32_t IDE3380_register_access::SPI_read_register(uint8_t register_address) {
        if (register_address >= IDE3380_register_count)return 0;
        // uint32_t valid_mask = ~((~0) << IDE3380_register_width[register_address]);
        // uint8_t data_rx[5];
        // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_SET);
        // uint8_t data_tx[5] = {0};
        // data_tx[0] = (register_address << 1) | 0x00; //read
        // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_RESET);
        // the_transfer_complete_flag = false;
        // HAL_SPI_TransmitReceive_DMA(&hspi1, data_tx, data_rx, 5);
        // while (!the_transfer_complete_flag) {
        //     HAL_Delay(1);
        // }
        // HAL_GPIO_WritePin(IDE3380_SPI1_nCS_GPIO_Port, IDE3380_SPI1_nCS_Pin, GPIO_PIN_SET);
        // return (data_rx[1] << 24 | data_rx[2] << 16 | data_rx[3] << 8 | data_rx[4]) & valid_mask;
        return 0;
    }
}
