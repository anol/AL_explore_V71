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
* @file   WIN32_print_support.cpp
* @author AndersEmilOlsen, IDEAS
* @date   11.02.2026
* @brief  
*/


#include "WIN32_print_support.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>


void vprint(const char *fmt, va_list argp) {
    enum { Print_buffer_size = 256 };
    static char vs_string[Print_buffer_size]{};
    if (0 < vsprintf(vs_string, fmt, argp)) {
        std::cout << vs_string;
    }
}

extern "C" void print(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
}

namespace Platform {
} // Platform
