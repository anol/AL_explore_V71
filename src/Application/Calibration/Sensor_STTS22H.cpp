#include "Sensor_STTS22H.h"

/*
   These static volatile flags are used to signal when a transmit or receive operation
   has completed (or when an error occurs).
*/
static volatile uint8_t smbusTxComplete = 0;
static volatile uint8_t smbusRxComplete = 0;
static volatile uint8_t smbusError    = 0;

/* Callback implementations – these override the weak HAL callbacks.
   They check that the callback is for our global handle hsmbus3.
*/
void HAL_SMBUS_MasterTxCpltCallback(SMBUS_HandleTypeDef *hsmbus)
{
    if(hsmbus == &hsmbus3)
    {
        smbusTxComplete = 1;
    }
}

void HAL_SMBUS_MasterRxCpltCallback(SMBUS_HandleTypeDef *hsmbus)
{
    if(hsmbus == &hsmbus3)
    {
        smbusRxComplete = 1;
    }
}

void HAL_SMBUS_ErrorCallback(SMBUS_HandleTypeDef *hsmbus)
{
    if(hsmbus == &hsmbus3)
    {
        smbusError = 1;
    }
}

/* Helper function: perform a memory-read transaction using interrupts.
   First, the register address is transmitted, then the data is received.
*/
static HAL_StatusTypeDef STTS22H_Mem_Read_IT(uint8_t reg, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef ret;

    smbusTxComplete = 0;
    smbusRxComplete = 0;
    smbusError    = 0;

    // Transmit the register address.
    ret = HAL_SMBUS_Master_Transmit_IT(&hsmbus3, (STTS22H_ADDR | 0x01), &reg, 1, SMBUS_XFEROPTIONS);
    if(ret != HAL_OK)
    {
        return ret;
    }
    // Wait (blocking) until the transmit is complete.
    while((smbusTxComplete == 0) && (smbusError == 0)) { }
    if(smbusError)
    {
        return HAL_ERROR;
    }

    // Now receive the data from the specified register.
    ret = HAL_SMBUS_Master_Receive_IT(&hsmbus3, STTS22H_ADDR, pData, Size, SMBUS_XFEROPTIONS);
    if(ret != HAL_OK)
    {
        return ret;
    }
    while((smbusRxComplete == 0) && (smbusError == 0)) { }
    if(smbusError)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/* Helper function: perform a memory-write transaction using interrupts.
   The register address and the data to write are combined into one buffer.
*/
static HAL_StatusTypeDef STTS22H_Mem_Write_IT(uint8_t reg, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef ret;
    smbusTxComplete = 0;
    smbusError    = 0;

    // For simplicity, we assume Size is small enough to fit in our temporary buffer.
    uint8_t buffer[16];
    if(Size + 1u > sizeof(buffer))
    {
        return HAL_ERROR;
    }
    buffer[0] = reg;
    for(uint16_t i = 0; i < Size; i++)
    {
        buffer[i + 1] = pData[i];
    }

    ret = HAL_SMBUS_Master_Transmit_IT(&hsmbus3, STTS22H_ADDR, buffer, Size + 1, SMBUS_XFEROPTIONS);
    if(ret != HAL_OK)
    {
        return ret;
    }
    while((smbusTxComplete == 0) && (smbusError == 0)) { }
    if(smbusError)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/*
   STTS22H_Init verifies communication with the sensor by reading the WHOAMI register.
*/
HAL_StatusTypeDef STTS22H_Init(void)
{
    uint8_t whoami;
    HAL_StatusTypeDef ret;

    ret = STTS22H_Mem_Read_IT(STTS22H_REG_WHOAMI, &whoami, 1);
    if(ret != HAL_OK)
    {
        return ret;
    }
    // Optionally: compare 'whoami' to an expected value.
    return HAL_OK;
}

/*
   STTS22H_TriggerOneShot sets the ONE_SHOT bit (bit0 of the CTRL register)
   to trigger a one-shot temperature conversion.
*/
HAL_StatusTypeDef STTS22H_TriggerOneShot(void)
{
    uint8_t ctrl;
    HAL_StatusTypeDef ret;

    // Read the current CTRL register.
    ret = STTS22H_Mem_Read_IT(STTS22H_REG_CTRL, &ctrl, 1);
    if(ret != HAL_OK)
    {
        return ret;
    }
    // Set the ONE_SHOT bit (bit 0).
    ctrl |= 0x01;
    ret = STTS22H_Mem_Write_IT(STTS22H_REG_CTRL, &ctrl, 1);
    return ret;
}

/*
   STTS22H_ReadStatus reads the STATUS register.
   In one-shot mode the BUSY bit (bit0) indicates if a conversion is in progress.
*/
HAL_StatusTypeDef STTS22H_ReadStatus(uint8_t *status)
{
    return STTS22H_Mem_Read_IT(STTS22H_REG_STATUS, status, 1);
}

/*
   STTS22H_ReadTemperature reads the two temperature registers (LSB and MSB)
   and converts the 16‑bit two’s complement value to a Celsius temperature.
   The sensor outputs temperature in hundredths of a degree.
*/
HAL_StatusTypeDef STTS22H_ReadTemperature(int16_t *temperature)
{
    HAL_StatusTypeDef ret;
    uint8_t temp_l, temp_h;

    ret = STTS22H_Mem_Read_IT(STTS22H_REG_TEMP_L_OUT, &temp_l, 1);
    if(ret != HAL_OK)
    {
        return ret;
    }
    ret = STTS22H_Mem_Read_IT(STTS22H_REG_TEMP_H_OUT, &temp_h, 1);
    if(ret != HAL_OK)
    {
        return ret;
    }

    int16_t raw_temp = (int16_t)((temp_h << 8) | temp_l);
    *temperature = raw_temp;
    return HAL_OK;
}

/*
   STTS22H_ReadTemperatureOneShot triggers a one-shot conversion,
   polls the STATUS register until the conversion is complete, then reads the temperature.
*/
HAL_StatusTypeDef STTS22H_ReadTemperatureOneShot(int16_t *temperature)
{
    HAL_StatusTypeDef ret;
    uint8_t status;

    ret = STTS22H_TriggerOneShot();
    if(ret != HAL_OK)
    {
        return ret;
    }

    // Poll until the BUSY bit (bit 0) is cleared.
    do
    {
        ret = STTS22H_ReadStatus(&status);
        if(ret != HAL_OK)
        {
            return ret;
        }
    } while (status & 0x01);

    ret = STTS22H_ReadTemperature(temperature);
    return ret;
}
