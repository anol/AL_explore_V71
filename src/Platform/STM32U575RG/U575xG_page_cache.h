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
* @file   U575xG_page_cache.h
* @author AndersEmilOlsen, IDEAS
* @date   25.02.2026
* @brief  
*/


#ifndef STM32U575RG_U575XG_PAGE_CACHE_H
#define STM32U575RG_U575XG_PAGE_CACHE_H
#include <cstdint>
#include <cstring>

#include "U575xG_embedded_flash.h"

namespace STM32U575RG {
    class U575xG_page_cache {
    public:
        enum {
            Page_size_8 = U575xG_embedded_flash::Page_size_8,
            Page_size_32 = Page_size_8 / sizeof(uint32_t),
            Quadword_width_32 = 4,
            Quadword_width_8 = Quadword_width_32 * sizeof(uint32_t),
            Quadwords_per_page = Page_size_8 / Quadword_width_8,
        };

        enum Entry_state : uint8_t {
            Is_unknown,
            Is_fail,
            Is_free,
            Is_same,
            Is_used,
            Is_void,
            To_clear,
            To_write,
        };

    private:
        static_assert(Page_size_32 == 2048, "Wrong page size in 32-bit words.");
        static_assert(Quadwords_per_page == 512, "Wrong max number of entries.");

        uint32_t the_page_cache[Page_size_32]{};
        Entry_state the_state[Quadwords_per_page]{};
        int the_clear_cnt{};
        int the_fail_cnt{};
        int the_free_cnt{};
        int the_same_cnt{};
        int the_used_cnt{};
        int the_void_cnt{};
        int the_write_cnt{};

    public:
        void initialize() {
            memset(the_page_cache, 0, sizeof(the_page_cache));
            memset(the_state, Is_unknown, sizeof(the_state));
            clear_diagnostics();
        }

        void set_state(const uint32_t entry, const Entry_state state) {
            if (entry < Quadwords_per_page) the_state[entry] = state;
            diagnostic_counting(state);
        }

        [[nodiscard]] Entry_state get_state(const uint32_t entry) const {
            return entry < Quadwords_per_page ? the_state[entry] : Is_fail;
        }

        [[nodiscard]] uint32_t *get_page() {
            return the_page_cache;
        }

        [[nodiscard]] int get_clear_cnt() const { return the_clear_cnt; }
        [[nodiscard]] int get_fail_cnt() const { return the_fail_cnt; }
        [[nodiscard]] int get_free_cnt() const { return the_free_cnt; }
        [[nodiscard]] int get_same_cnt() const { return the_same_cnt; }
        [[nodiscard]] int get_used_cnt() const { return the_used_cnt; }
        [[nodiscard]] int get_void_cnt() const { return the_void_cnt; }
        [[nodiscard]] int get_write_cnt() const { return the_write_cnt; }

    private:
        void clear_diagnostics() {
            the_clear_cnt = 0;
            the_fail_cnt = 0;
            the_fail_cnt = 0;
            the_free_cnt = 0;
            the_used_cnt = 0;
            the_void_cnt = 0;
            the_write_cnt = 0;
        }

        void diagnostic_counting(const Entry_state state) {
            switch (state) {
                case Is_fail: the_fail_cnt++;
                    break;
                case Is_free: the_free_cnt++;
                    break;
                case Is_same: the_same_cnt++;
                    break;
                case Is_used: the_used_cnt++;
                    break;
                case Is_void: the_void_cnt++;
                    break;
                case To_clear: the_clear_cnt++;
                    break;
                case To_write: the_write_cnt++;
                    break;
                default: break;
            }
        }
    };
} // STM32U575RG

#endif //STM32U575RG_U575XG_PAGE_CACHE_H
