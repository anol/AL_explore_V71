/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 * \date   IDEAS/23.03.2020/anolsen
 * \brief
 */

#include <cstdio>

#include "Common_stdio.h"

extern "C" {
//! Pointer to the base of the USART module instance to use for stdio.
volatile void *volatile stdio_base;
//! Pointer to the external low level write function.
int (*ptr_put)(void volatile *, char);
//! Pointer to the external low level read function.
void (*ptr_get)(void volatile *, char *);
}

static Abstract_UART *optional_one_and_only_console = nullptr;

extern "C" int stdio_serial_putchar(void volatile *, char c) {
    if (optional_one_and_only_console != nullptr) {
        optional_one_and_only_console->put(c);
        return 1;
    }
    return -1;
}

extern "C" int _write(int file, const char *ptr, int len) {
    if (optional_one_and_only_console != nullptr) {
        return optional_one_and_only_console->print(ptr, len);
    }
    return -1;
}

extern "C" void stdio_serial_getchar(void volatile *, char *data) {
    *data = '!';
}

extern "C" int _read(int file, char *ptr, int len) {
    return -1;
}

void Platform::Common_stdio::initialize() const {
    stdio_base = nullptr;
    ptr_put = &stdio_serial_putchar;
    ptr_get = &stdio_serial_getchar;
    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);
    optional_one_and_only_console = optional_UART;
}
