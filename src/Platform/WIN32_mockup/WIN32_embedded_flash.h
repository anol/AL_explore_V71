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
* @file   WIN32_embedded_flash.h
* @author AndersEmilOlsen, IDEAS
* @date   04.02.2026
* @brief  
*/


#ifndef UNIT_TEST_EMBEDDED_FLASH_H
#define UNIT_TEST_EMBEDDED_FLASH_H
#include <cstdint>

#include "Repository/Embedded_flash.h"

namespace WIN32_mockup {
    class WIN32_embedded_flash : public Repository::Embedded_flash {
        enum:uint32_t {
            Flash_bank1 = 1, Flash_bank2 = 2, Pages_per_bank = 64,
            Page_size_8 = 8192, Page_size_32 = Page_size_8 / sizeof(uint32_t),
            Left_page = 62,
            Right_page = 63,
        };

        constexpr static char Left_file_name[]{"left_file.bin"};
        constexpr static char Right_file_name[]{"right_file.bin"};
        static uint32_t the_left_page[Page_size_32];
        static uint32_t the_right_page[Page_size_32];
        bool is_left_current{};
        int cnt_left_pages{};
        int cnt_right_pages{};
        int cnt_swap_pages{};

        static bool is_write_open;
        // static uint32_t last_error_status;

    public:
        WIN32_embedded_flash();

        void open_for_write(bool open) override { is_write_open = open; }

        [[nodiscard]] bool is_open_for_write() override { return is_write_open; }

        [[nodiscard]] bool erase_page(uint32_t bank, uint32_t page) override;

        [[nodiscard]] bool read_quad(const uint32_t* address, uint32_t* quadword) override;

        [[nodiscard]] bool program_quad(uint32_t* address, const uint32_t quadword[4]) override;

        void read_page(uint32_t* buffer, const uint32_t* page, uint32_t size_8) override;

        void begin_programming() override;

        void end_programming() override;

        [[nodiscard]] uint32_t* get_left_page() override { return the_left_page; };

        [[nodiscard]] uint32_t* get_right_page() override { return the_right_page; };

        [[nodiscard]] bool erase_left_page() override { return erase_page(Flash_bank2, Left_page); };

        [[nodiscard]] bool erase_right_page() override { return erase_page(Flash_bank2, Right_page); };

        [[nodiscard]] bool assert_address(const uint32_t* address) const override;

        void print_diag() override;
    };
} // WIN32_mockup

#endif //UNIT_TEST_EMBEDDED_FLASH_H
