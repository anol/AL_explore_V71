/*
* Copyright (C) 2025-2026 Integrated Detector Electronics AS
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
* @file   U575xG_persistent_storage.h
* @author AndersEmilOlsen, IDEAS
* @date   04.02.2026
* @brief  
*/


#ifndef UNIT_TEST_U575XG_PERSISTENT_STORAGE_H
#define UNIT_TEST_U575XG_PERSISTENT_STORAGE_H

#include "U575xG_page_cache.h"
#include "Repository/Embedded_flash.h"
#include "Repository/Persistent_storage.h"

namespace STM32U575RG {
    class U575xG_persistent_storage : public Repository::Persistent_storage {
    public:
        enum : uint32_t {
            Index_of_id = 0,
            Index_of_sentinel = 1,
            Index_of_size = 2,
            Index_of_value = 3,
            Void_value = 0x0000'0000,
            Free_to_use = 0xFFFF'FFFF,
            Sentinel_value = 0xABBA'BABE,
            Default_value_size_8 = 4,
        };

    private:
        enum : uint8_t { Void_result, Success_result, Error_result };

        U575xG_page_cache the_cache;
        Repository::Embedded_flash &use_flash;
        uint32_t *optional_page_address{};
        bool is_left_current{};

    public:
        explicit U575xG_persistent_storage(Repository::Embedded_flash &flash) : use_flash(flash) {
        }

        [[nodiscard]] bool initialize() override;

        [[nodiscard]] bool clean() override;

        [[nodiscard]] bool open_reading() override;

        [[nodiscard]] bool read(uint32_t id, int32_t &value) override;

        [[nodiscard]] bool open_writing(int dirty_count) override;

        [[nodiscard]] bool write_cache(uint32_t id, int32_t value) override;

        [[nodiscard]] bool program_flash() override;

        void dump() const override;

        void print_diag() const override;

        static uint32_t get_page_size() { return U575xG_page_cache::Page_size_8; }

    private:
        [[nodiscard]] bool swap_pages();

        [[nodiscard]] uint32_t *current_page_address() const { return optional_page_address; };

        bool load_cache(const uint32_t *page);

        bool write_from_cache_to_flash(uint32_t *page_address);

        bool copy_page(const uint32_t *source_addr, uint32_t *target_addr) const;

        bool search_value(uint32_t id, int32_t &value, const uint32_t *address) const;

        bool write_to_cache(uint32_t id, int32_t value);

        static void assert_id(uint32_t id, uint8_t &result);
    };
} // STM32U575RG

#endif //UNIT_TEST_U575XG_PERSISTENT_STORAGE_H
