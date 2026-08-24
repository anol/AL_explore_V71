//
// Created by aeols on 2026-08-19.
//

#pragma once

#include <cstdint>

namespace Application_configuration
{
        enum Pin_id : std::uint8_t {
            Pin_not_used,

            Pin_CAN_LOOPBK0,
            Pin_CAN_LOOPBK1,
            Pin_CAN_RX0,
            Pin_CAN_RX1,
            Pin_CAN_STBY0,
            Pin_CAN_STBY1,
            Pin_CAN_TX0,
            Pin_CAN_TX1,
            Pin_DetBias_Clk,
            Pin_DGU_bias,
            Pin_DGU_DCAL,
            Pin_DGU_DRESET,
            Pin_DGU_DTVO_BUF,
            Pin_DGU_ERR,
            Pin_DGU_LU1,
            Pin_DGU_LU2,
            Pin_DGU_MSTEST,
            Pin_DGU_Power_ENb,
            Pin_DGU_RO_CLK,
            Pin_DGU_SHIFT_IN,
            Pin_DGU_SHIFT_OUT,
            Pin_DGU_SPI_CS0,
            Pin_DGU_SPI_CS1,
            Pin_DGU_SPI_CS2,
            Pin_DGU_SPI_MISO,
            Pin_DGU_SPI_MOSI,
            Pin_DGU_SPI_SCK,
            Pin_DGU_SRESET,
            Pin_DGU_SS_HOLD,
            Pin_DGU_THGVO_1,
            Pin_DGU_THGVO_2,
            Pin_DGU_TLGVO,
            Pin_DOSI_MUX_A0,
            Pin_DOSI_MUX_A1,
            Pin_DOSI_MUX_A2,
            Pin_DOSI_MUX_EN,
            Pin_DOSI_RO_EN,
            Pin_I2C_CK,
            Pin_I2C_D,

            Number_of_pins
        };

} // Application_configuration
