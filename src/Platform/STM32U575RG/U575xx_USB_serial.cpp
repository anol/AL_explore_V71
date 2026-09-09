/*
* Copyright (C) 2020-2026 Integrated Detector Electronics AS
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
* @file   U575xx_USB_serial.cpp
* @author AndersEmilOlsen, IDEAS
* @date   05.02.2026
* @brief  
*/

#include <cstdarg>

#include "U575xx_USB_serial.h"

#include "stm32u5xx_hal.h"
#include "ux_api.h"
#include "ux_device_class_cdc_acm.h"
#include "usart.h"

extern UX_SLAVE_CLASS_CDC_ACM *cdc_acm;

static uint32_t the_count_busy_tx = 0;
static uint32_t the_count_RxHalfCpltCallback = 0;
static uint32_t the_count_RxCpltCallback = 0;
static char vs_string[STM32U575RG::U575xx_USB_serial::UART_BUFFER_TX_SIZE]{};

volatile uint8_t RX_buffer[30] = {0};
volatile uint8_t rx_buffer_index = 0;
volatile uint8_t TxCplt = 1;

extern "C" void cdc_acm_rx_callback_f() {
    uint32_t status;
    uint32_t actual_length;
    uint8_t received_char[64] = {0};
    if (_ux_system_slave->ux_system_slave_device.ux_slave_device_state != UX_DEVICE_CONFIGURED) {
        return;
    }
    //check if USB have data to read
    auto *cdc_acm_temp = cdc_acm;
    if (cdc_acm_temp == nullptr)return;
    if (cdc_acm_temp->ux_device_class_cdc_acm_read_state != UX_STATE_RESET) {
    }
    status = ux_device_class_cdc_acm_read_run(cdc_acm_temp, received_char, 64, &actual_length);
    if (status == UX_STATE_NEXT && actual_length >= 1) {
        for (uint32_t gpc = 0; gpc < actual_length; gpc++) {
            STM32U575RG::U575xx_USB_serial::on_rx(received_char[gpc]);
        }
        received_char[actual_length] = 0;
        USB_OTG_FS->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;
    }
}

extern "C" void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *) {
    the_count_RxHalfCpltCallback++;
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *) {
    the_count_RxCpltCallback++;
    STM32U575RG::U575xx_USB_serial::on_rx(RX_buffer[0]);
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    TxCplt = 1;
}

uint32_t send_usb_string(UCHAR String[], uint32_t Length) {
    if (cdc_acm == nullptr) {
        return UX_STATE_ERROR; // Ensure the CDC ACM instance is valid before sending
    }
    uint32_t actual_length;
    // Keep calling the write function until the transfer is complete or an error is returned.
    uint32_t timeout = 10;
    uint32_t time = HAL_GetTick();
    uint32_t status = ux_device_class_cdc_acm_write_run(cdc_acm, String, Length, &actual_length);
    do {
        status = ux_device_class_cdc_acm_write_run(cdc_acm, nullptr, 0, &actual_length);
    } while (status == UX_STATE_WAIT && HAL_GetTick() - time < timeout);
    return status;
}

void vprint(const char *fmt, va_list argp) {
    if (0 < vsprintf(vs_string, fmt, argp)) {
        send_usb_string((UCHAR *) vs_string, strlen(vs_string));
    }
}

extern "C" void print(const char *fmt, ...) {
    if (cdc_acm == nullptr)return;
    if (cdc_acm->ux_slave_class_cdc_acm_data_rts_state != UX_TRUE) {
        the_count_busy_tx++;
        return;
    }
    TxCplt = 0;
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
    while (cdc_acm->ux_slave_class_cdc_acm_data_rts_state != UX_TRUE);
}

uint32_t UART_driver_get_count_busy_tx() { return the_count_busy_tx; }
uint32_t UART_driver_get_count_RxHalfCpltCallback() { return the_count_RxHalfCpltCallback; }
uint32_t UART_driver_get_count_RxCpltCallback() { return the_count_RxCpltCallback; }

namespace STM32U575RG {
    void *U575xx_USB_serial::optional_serial_user{};
    U575xx_USB_serial::serial_func U575xx_USB_serial::optional_serial_func{};

    char U575xx_USB_serial::AT_buffer[UART_BUFFER_RX_SIZE]{};

    void U575xx_USB_serial::initialize(void *user, const serial_func func) {
        optional_serial_user = user;
        optional_serial_func = func;
        open_rx();
    }

    void U575xx_USB_serial::open_rx() {
        rx_buffer_index = 0;
        HAL_UART_Receive_DMA(&huart1, (uint8_t *) RX_buffer, 1);
    }

    void U575xx_USB_serial::get_state() {
        cdc_acm_rx_callback_f();
    }

    void U575xx_USB_serial::on_rx(uint8_t data) {
        if (optional_serial_func) {
            optional_serial_func(optional_serial_user, data);
        }
    }
} // STM32U575RG
