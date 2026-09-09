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
* @file   Binary_file.h
* @author AndersEmilOlsen, IDEAS
* @date   03.02.2026
* @brief  
*/


#ifndef WIN32_BINARY_FILE_H
#define WIN32_BINARY_FILE_H
#include <cstdint>

namespace WIN32_mockup {
    class WIN32_binary_file {
    public:
        static void get_page(const char* file_name, uint32_t* data_32, int size_32);

        static void put_page(const char* file_name, const uint32_t* data_32, int size_32);

        static void get_page(const char* file_name, uint8_t* data_8, int size);

        static void put_page(const char* file_name, const uint8_t* data_8, int size);
    };
} // WIN32_mockup

#endif //WIN32_BINARY_FILE_H
