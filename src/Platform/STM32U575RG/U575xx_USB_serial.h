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
* @file   U575xx_USB_serial.h
* @author AndersEmilOlsen, IDEAS
* @date   05.02.2026
* @brief  
*/


#ifndef UNIT_TEST_U575_USB_SERIAL_H
#define UNIT_TEST_U575_USB_SERIAL_H

namespace STM32U575RG {
    class U575xx_USB_serial {
    public:
        enum { UART_BUFFER_RX_SIZE = 50, UART_BUFFER_TX_SIZE = 150, };

        using serial_func = void (*)(void *, uint8_t);

    private:
        static void *optional_serial_user;

        static serial_func optional_serial_func;

        static char AT_buffer[UART_BUFFER_RX_SIZE];

    public:
        static void initialize(void *user, serial_func func);

        static void open_rx();

        static void get_state();

        static void on_rx(uint8_t data);
    };
} // STM32U575RG

#endif //UNIT_TEST_U575_USB_SERIAL_H
