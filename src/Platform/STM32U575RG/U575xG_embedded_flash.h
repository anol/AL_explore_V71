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
* @file   U575xG_embedded_flash.h
* @author AndersEmilOlsen, IDEAS
* @date   04.02.2026
* @brief  
*/


#ifndef STM32U575RG_U575XG_EMBEDDED_FLASH_H
#define STM32U575RG_U575XG_EMBEDDED_FLASH_H
import Support.Embedded_flash;

namespace STM32U575RG {
    class U575xG_embedded_flash : public Repository::Embedded_flash {
    public:
        enum:uint32_t {
            Flash_bank1 = 1,
            Flash_bank2 = 2, Flash_bank2_base = 0x0808'0000, End_of_flash = 0x080F'FFFF,
            Page_size_8 = 8192, Page_size_32 = Page_size_8 / sizeof(uint32_t),
            Left_page = 60, Left_page_base = Flash_bank2_base + Left_page * Page_size_8,
            Right_page = 61, Right_page_base = Flash_bank2_base + Right_page * Page_size_8,
            Pages_per_bank = 64,
        };

    private:
        enum:uint32_t {
            Unlock_key_1 = 0x4567'0123, Unlock_key_2 = 0xCDEF'89AB,
            Operation_timeout_1s = 1000, // Timer ticks
            Operation_timeout_2s = 2000, // Timer ticks
            Operation_timeout_100ms = 100, // Timer ticks
            Operation_timeout_10ms = 10, // Timer ticks
            Countdown_for_control = 1'000'000, // Busy loop
            Countdown_for_ceased = 10'000'000, // Busy loop
            Countdown_for_raised = 40'000'000, // Busy loop
        };

        static bool is_write_open;
        // static uint32_t last_error_status;

        static_assert(Left_page_base == 0x080F'8000, "Flash page 60 address error.");
        static_assert(Right_page_base == 0x080F'A000, "Flash page 61 address error.");
        // static_assert(Left_page_base == 0x080F'C000, "Flash page 62 address error.");
        // static_assert(Right_page_base == 0x080F'E000, "Flash page 63 address error.");
        static_assert(End_of_flash == Flash_bank2_base + 64 * Page_size_8 - 1, "Flash addr error.");

    public:
        void open_for_write(bool open) override { is_write_open = open; }

        [[nodiscard]] bool is_open_for_write() override { return is_write_open; }

        [[nodiscard]] Status_code erase_page(uint32_t bank, uint32_t page) override;

        [[nodiscard]] Status_code read_quad(const uint32_t *address, uint32_t *quadword) override;

        [[nodiscard]] Status_code program_quad(uint32_t *address, const uint32_t quadword[4]) override;

        void read_page(uint32_t *buffer, const uint32_t *page, uint32_t size_8) override;

        void begin_programming() override;

        void end_programming() override;

        void print_diag() override;

        [[nodiscard]] uint32_t *get_left_page() override;

        [[nodiscard]] uint32_t *get_right_page() override;

        [[nodiscard]] Status_code erase_left_page() override;

        [[nodiscard]] Status_code erase_right_page() override;

        [[nodiscard]] bool assert_address(const uint32_t *address) const override;

    private:
        static bool check_status_flags(uint32_t status_flags);

        [[nodiscard]] static Status_code busy_wait_for_raised_flag(uint32_t flags);

        [[nodiscard]] static Status_code busy_wait_for_ceased_flag(uint32_t flags);

        [[nodiscard]] static Status_code busy_wait_for_control_flag(uint32_t flags);

        [[nodiscard]] static Status_code wait_for_complete(uint32_t timeout);

        static void wait_for_timeout(uint32_t timeout);

        [[nodiscard]] static Status_code unlock_control_register();

        static void clear_error_status();

        static void clear_end_of_operation();
    };
} // STM32U575RG

#endif //STM32U575RG_U575XG_EMBEDDED_FLASH_H
