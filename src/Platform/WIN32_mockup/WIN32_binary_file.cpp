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
* @file   Binary_file.cpp
* @author AndersEmilOlsen, IDEAS
* @date   03.02.2026
* @brief  
*/


#include "WIN32_binary_file.h"

#include <fstream>
#include <iostream>

namespace WIN32_mockup {
    void WIN32_binary_file::get_page(const char* file_name, uint32_t* data_32, const int size_32)
    {
        const auto size_8 = size_32 * sizeof(uint32_t);
        std::ifstream input{};
        input.open(file_name, std::ios::binary | std::ios::in);
        input.read(reinterpret_cast<char*>(data_32), size_8);
        input.close();
    }

    void WIN32_binary_file::put_page(const char* file_name, const uint32_t* data_32, const int size_32)
    {
        const auto size_8 = size_32 * sizeof(uint32_t);
        std::ofstream output{};
        output.open(file_name, std::ios::binary | std::ios::out);
        output.write(reinterpret_cast<const char*>(data_32), size_8);
        output.close();
    }

    void WIN32_binary_file::get_page(const char* file_name, uint8_t* data_8, const int size_8)
    {
        std::ifstream input{};
        input.open(file_name, std::ios::binary | std::ios::in);
        input.read(reinterpret_cast<char*>(data_8), size_8);
        input.close();
    }

    void WIN32_binary_file::put_page(const char* file_name, const uint8_t* data_8, const int size_8)
    {
        std::ofstream output{};
        output.open(file_name, std::ios::binary | std::ios::out);
        output.write(reinterpret_cast<const char*>(data_8), size_8);
        output.close();
    }
} // Utility
