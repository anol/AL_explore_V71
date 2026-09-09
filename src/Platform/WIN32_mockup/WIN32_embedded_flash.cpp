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
* @file   WIN32_embedded_flash.cpp
* @author AndersEmilOlsen, IDEAS
* @date   04.02.2026
* @brief  
*/

#include "WIN32_embedded_flash.h"

#include <cstring>

#include <stdio.h>
#include "WIN32_binary_file.h"

extern uint32_t ld_repository_addr;
extern uint32_t ld_repository_size;

namespace WIN32_mockup {
    uint32_t cnt_alignment_error{};
    uint32_t cnt_busy_not_cleared{};
    uint32_t cnt_clear_errors{};
    uint32_t cnt_end_of_operation{};
    uint32_t cnt_erase{};
    uint32_t cnt_new_error{};
    uint32_t cnt_old_error{};
    uint32_t cnt_operation_error{};
    uint32_t cnt_page_erase_error{};
    uint32_t cnt_programmed{};
    uint32_t cnt_programming_error{};
    uint32_t cnt_sequence_error{};
    uint32_t cnt_size_error{};
    uint32_t cnt_status_error{};
    uint32_t cnt_timeout_error{};
    uint32_t cnt_unlock_error{};
    uint32_t cnt_wait_ceased_failed{};
    uint32_t cnt_wait_ceased{};
    uint32_t cnt_wait_CR_failed{};
    uint32_t cnt_wait_CR{};
    uint32_t cnt_wait_raised_failed{};
    uint32_t cnt_wait_raised{};
    uint32_t cnt_write_option_error{};
    uint32_t cnt_write_protection_error{};

    enum Control_flags: uint32_t {
        Control_flags_to_clear = 0,
        Control_flags_for_erase = 0,
        Control_flags_for_program = 0,
        Status_flags_to_clear = 0,
    };

    bool WIN32_embedded_flash::is_write_open{};

    WIN32_embedded_flash::WIN32_embedded_flash()
    {
        WIN32_binary_file::get_page(Left_file_name, the_left_page, Page_size_32);
        WIN32_binary_file::get_page(Right_file_name, the_right_page, Page_size_32);
    }

    uint32_t WIN32_embedded_flash::the_right_page[]{};
    uint32_t WIN32_embedded_flash::the_left_page[]{};

    void WIN32_embedded_flash::read_page(uint32_t* buffer, const uint32_t* page, const uint32_t size_8)
    {
        memcpy(buffer, page, size_8);
    }

    bool WIN32_embedded_flash::read_quad(const uint32_t* address, uint32_t* quadword)
    {
        *quadword++ = *address++;
        *quadword++ = *address++;
        *quadword++ = *address++;
        *quadword = *address;
        return true;
    }

    void WIN32_embedded_flash::begin_programming()
    {}

    void WIN32_embedded_flash::end_programming()
    {
        if (is_left_current) {
            WIN32_binary_file::put_page(Left_file_name, the_left_page, Page_size_32);
        }
        else {
            WIN32_binary_file::put_page(Right_file_name, the_right_page, Page_size_32);
        }
    }

    bool WIN32_embedded_flash::erase_page(const uint32_t bank, const uint32_t page)
    {
        constexpr uint32_t Erased_value{0xFF};
        const auto success{bank == Flash_bank2};
        if (page == Left_page) {
            memset(the_left_page, Erased_value, Page_size_8);
            WIN32_binary_file::put_page(Left_file_name, reinterpret_cast<const uint8_t*>(the_left_page), Page_size_8);
        }
        else {
            memset(the_right_page, Erased_value, Page_size_8);
            WIN32_binary_file::put_page(Right_file_name, reinterpret_cast<const uint8_t*>(the_right_page), Page_size_8);
        }
        cnt_erase++;
        return success;
    }

    bool WIN32_embedded_flash::program_quad(uint32_t* address, const uint32_t quadword[4])
    {
        auto success{address != nullptr};
        if (success) {
            is_left_current = address < the_right_page;
            *address++ = *quadword++;
            *address++ = *quadword++;
            *address++ = *quadword++;
            *address = *quadword;
            cnt_programmed++;
        }
        return success;
    }

    bool WIN32_embedded_flash::assert_address(const uint32_t* address) const
    {
        const auto success{address != nullptr};
        return success;
    }

    void WIN32_embedded_flash::print_diag()
    {
        printf(" Flash: mode=%s\r\n", is_open_for_write() ? "write" : "read");
        printf("  Count: program=%d, erase=%d \r\n", cnt_programmed, cnt_erase);
    }
} // STM32U575RG
