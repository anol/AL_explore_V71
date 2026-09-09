#ifndef __STTS22H_H
#define __STTS22H_H

#include "stm32U5xx_hal.h"       // Adjust for your STM32 family
#include "stm32U5xx_hal_smbus.h" // Required for SMBUS_HandleTypeDef

extern SMBUS_HandleTypeDef hsmbus3; // Global SMBus handle defined in main.c

// For Addr tied to GND the 7‑bit address is 0x3F.
// HAL functions expect the address left-shifted by 1.
#define STTS22H_ADDR        (0x3F << 1)

// Define transfer options – for a single, complete transfer we use FIRST_AND_LAST_FRAME.
//#define I2C_FIRST_AND_LAST_FRAME 0U
#define SMBUS_XFEROPTIONS   (SMBUS_FIRST_AND_LAST_FRAME_NO_PEC)

/* Register addresses */
#define STTS22H_REG_WHOAMI       0x01
#define STTS22H_REG_TEMP_H_LIMIT 0x02
#define STTS22H_REG_TEMP_L_LIMIT 0x03
#define STTS22H_REG_CTRL         0x04
#define STTS22H_REG_STATUS       0x05
#define STTS22H_REG_TEMP_L_OUT   0x06
#define STTS22H_REG_TEMP_H_OUT   0x07

/* Function prototypes */
HAL_StatusTypeDef STTS22H_Init(void);
HAL_StatusTypeDef STTS22H_TriggerOneShot(void);
HAL_StatusTypeDef STTS22H_ReadStatus(uint8_t *status);
HAL_StatusTypeDef STTS22H_ReadTemperature(int16_t *temperature);
HAL_StatusTypeDef STTS22H_ReadTemperatureOneShot(int16_t *temperature);

#endif /* __STTS22H_H */
