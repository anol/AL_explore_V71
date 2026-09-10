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
* @file   U575xG_persistent_storage.cpp
* @author AndersEmilOlsen, IDEAS
* @date   04.02.2026
* @brief  
*/


#include "U575xG_persistent_storage.h"

#include <cstdio>
#include "U575xG_embedded_flash.h"
// #include "stm32u575xx.h"
#include "U575xG_page_cache.h"
#include "Repository/Attribute_type.h"

namespace STM32U575RG {
    static uint32_t cnt_copy_error{};
    static uint32_t cnt_copy{};
    static uint32_t cnt_erase_error{};
    static uint32_t cnt_left_pages{};
    static uint32_t cnt_lookup{};
    static uint32_t cnt_page_error{};
    static uint32_t cnt_param_error{};
    static uint32_t cnt_right_pages{};
    static uint32_t cnt_same{};
    static uint32_t cnt_swap_pages{};
    static uint32_t cnt_update_error{};
    static uint32_t cnt_update{};
    static uint32_t cnt_void_error{};
    static uint32_t cnt_void{};
    static uint32_t cnt_write_error{};
    static uint32_t cnt_write{};

    Status_code U575xG_persistent_storage::initialize() {
        const auto left_is_erased = Free_to_use == *use_flash.get_left_page();
        const auto right_is_erased = Free_to_use == *use_flash.get_right_page();
        if (left_is_erased) {
            is_left_current = false;
            optional_page_address = use_flash.get_right_page();
        } else if (right_is_erased) {
            is_left_current = true;
            optional_page_address = use_flash.get_left_page();
        }
        return Status_code::Success();
    }

    Status_code U575xG_persistent_storage::clean() {
        bool success = use_flash.erase_left_page().success();
        if (!use_flash.erase_right_page().success()) success = false;
        if (!success) {
            cnt_erase_error++;
        }
        return Status_code(success);
    }

    Status_code U575xG_persistent_storage::swap_pages() {
        bool success{};
        cnt_swap_pages++;
        if (is_left_current) {
            success = copy_page(the_cache.get_page(), use_flash.get_right_page()).success();
            if (success) {
                is_left_current = false;
                optional_page_address = use_flash.get_right_page();
                success = use_flash.erase_left_page().success();
            }
        } else {
            success = copy_page(the_cache.get_page(), use_flash.get_left_page()).success();
            if (success) {
                is_left_current = true;
                optional_page_address = use_flash.get_left_page();
                success = use_flash.erase_right_page().success();
            }
        }
        if (!success) {
            cnt_erase_error++;
        }
        return Status_code(success);
    }

    Status_code U575xG_persistent_storage::open_reading() {
        use_flash.open_for_write(false);
        const auto *page = current_page_address();
        load_cache(page);
        return Status_code::Success();
    }

    Status_code U575xG_persistent_storage::read(const uint32_t id, int32_t &value) {
        if (use_flash.is_open_for_write()) {
            printf("Not open for reading\r\n");
            return Status_code::Failure();
        }
        const auto *page = current_page_address();
        return search_value(id, value, page);
    }

    Status_code U575xG_persistent_storage::open_writing(const int dirty_count) {
        use_flash.open_for_write(false);
        auto success = load_cache(current_page_address()).success();
        if (success) {
            if (dirty_count >= the_cache.get_free_cnt()) {
                success = swap_pages().success();
                if (success) {
                    success = load_cache(current_page_address()).success();
                }
            }
        }
        if (success) {
            use_flash.open_for_write(true);
        }
        return Status_code(success);
    }

    Status_code U575xG_persistent_storage::write_cache(const uint32_t id, const int32_t value) {
        if (!use_flash.is_open_for_write()) {
            printf("Not open for writing\r\n");
            return Status_code::Failure();
        }
        const auto success = write_to_cache(id, value).success();
        if (success) {
            if (is_left_current) {
                cnt_left_pages++;
            } else {
                cnt_right_pages++;
            }
        }
        return Status_code(success);
    }

    Status_code U575xG_persistent_storage::program_flash() {
        auto *page = current_page_address();
        const auto success = write_from_cache_to_flash(page).success();
        use_flash.open_for_write(false);
        return Status_code(success);
    }

    Status_code U575xG_persistent_storage::load_cache(const uint32_t *page) {
        bool success{page != nullptr};
        if (success) {
            the_cache.initialize();
            auto *cache = the_cache.get_page();
            use_flash.read_page(cache, page, U575xG_page_cache::Page_size_8);
            for (uint32_t gpc = 0; gpc < U575xG_page_cache::Quadwords_per_page; gpc++) {
                auto cursor = cache[Index_of_id];
                if (Free_to_use == cursor) {
                    the_cache.set_state(gpc, U575xG_page_cache::Is_free);
                } else if (Void_value == cursor) {
                    the_cache.set_state(gpc, U575xG_page_cache::Is_void);
                } else {
                    if (Sentinel_value == cache[Index_of_sentinel]) {
                        the_cache.set_state(gpc, U575xG_page_cache::Is_used);
                    } else {
                        the_cache.set_state(gpc, U575xG_page_cache::Is_fail);
                    }
                }
                cache += U575xG_page_cache::Quadword_width_32;
            }
        }
        return Status_code(success);
    }

    Status_code U575xG_persistent_storage::write_to_cache(const uint32_t id, const int32_t value) {
        auto unsigned_value = static_cast<uint32_t>(value);
        uint8_t result{Void_result};
        auto *address = the_cache.get_page();
        assert_id(id, result);
        for (uint32_t gpc = 0; (Void_result == result) && (gpc < U575xG_page_cache::Quadwords_per_page); gpc++) {
            auto cursor = address[Index_of_id];
            if (id == cursor) {
                if (Sentinel_value == address[Index_of_sentinel]) {
                    if (unsigned_value == address[Index_of_value]) {
                        the_cache.set_state(gpc, U575xG_page_cache::Is_same);
                        cnt_same++;
                        result = Success_result;
                    } else {
                        the_cache.set_state(gpc, U575xG_page_cache::To_clear);
                        address[Index_of_id] = Void_value;
                        address[Index_of_sentinel] = Void_value;
                        address[Index_of_size] = Void_value;
                        address[Index_of_value] = Void_value;
                        address += U575xG_page_cache::Quadword_width_32;
                    }
                } else {
                    printf("Update: Illegal sentinel in quadword %d\r\n", gpc);
                    result = Error_result;
                }
            } else if (Free_to_use == cursor) {
                the_cache.set_state(gpc, U575xG_page_cache::To_write);
                address[Index_of_id] = id;
                address[Index_of_sentinel] = Sentinel_value;
                address[Index_of_size] = Default_value_size_8;
                address[Index_of_value] = unsigned_value;
                cnt_update++;
                result = Success_result;
            } else {
                cnt_lookup++;
                address += U575xG_page_cache::Quadword_width_32;
            }
        }
        return Status_code(Error_result != result);
    }

    Status_code U575xG_persistent_storage::write_from_cache_to_flash(uint32_t *page_address) {
        constexpr uint32_t void_quad[U575xG_page_cache::Quadword_width_32]{
            Void_value, Void_value, Void_value, Void_value
        };
        auto success{true};
        auto *cache_address = the_cache.get_page();
        set_diag_code(0);
        use_flash.begin_programming();
        for (uint32_t gpc = 0; success && (gpc < U575xG_page_cache::Quadwords_per_page); gpc++) {
            switch (the_cache.get_state(gpc)) {
                case U575xG_page_cache::Is_void:
                case U575xG_page_cache::Is_used:
                case U575xG_page_cache::Is_same:
                case U575xG_page_cache::Is_free:
                    break;
                case U575xG_page_cache::To_clear: {
                    if (use_flash.program_quad(page_address, void_quad).success()) {
                        cnt_void++;
                    } else {
                        cnt_void_error++;
                        set_diag_code(0x1UL << 8);
                        success = false;
                    }
                    break;
                }
                case U575xG_page_cache::To_write: {
                    if (use_flash.program_quad(page_address, cache_address).success()) {
                        cnt_write++;
                    } else {
                        cnt_write_error++;
                        set_diag_code(0x1UL << 9);
                        success = false;
                    }
                    break;
                }
                case U575xG_page_cache::Is_fail:
                default:
                    set_diag_code(0x1UL << 10);
                    success = false;
                    break;
            }
            cache_address += U575xG_page_cache::Quadword_width_32;
            page_address += U575xG_page_cache::Quadword_width_32;
        }
        use_flash.end_programming();
        return Status_code(success);
    }

    Status_code U575xG_persistent_storage::copy_page(const uint32_t *source_addr, uint32_t *target_addr) const {
        uint32_t quad_word[U575xG_page_cache::Quadword_width_32]{};
        for (uint32_t quad = 0; quad < U575xG_page_cache::Quadwords_per_page; quad++) {
            quad_word[Index_of_id] = *source_addr;
            if (Void_value != quad_word[Index_of_id]) {
                if (Free_to_use == quad_word[Index_of_id]) {
                    return Status_code::Success();
                }
                quad_word[Index_of_sentinel] = Sentinel_value;
                if (source_addr[Index_of_sentinel] != Sentinel_value) {
                    printf("Copy: Illegal sentinel at 0x%08X\r\n", source_addr);
                    return Status_code::Failure();
                }
                quad_word[Index_of_size] = source_addr[Index_of_size];
                quad_word[Index_of_value] = source_addr[Index_of_value];
                cnt_copy = cnt_copy + 1;
                if (!use_flash.program_quad(target_addr, quad_word).success()) {
                    cnt_copy_error = cnt_copy_error + 1;
                }
                target_addr += U575xG_page_cache::Quadword_width_32;
            }
            source_addr += U575xG_page_cache::Quadword_width_32;
        }
        return Status_code::Success();
    }

    Status_code U575xG_persistent_storage::search_value(const uint32_t id, int32_t &value, const uint32_t *address) const {
        uint8_t result{use_flash.assert_address(address) ? Void_result : Error_result};
        uint32_t read_quad[U575xG_page_cache::Quadword_width_32]{
        };
        assert_id(id, result);
        for (uint32_t gpc = 0; Void_result == result && gpc < U575xG_page_cache::Quadwords_per_page; gpc++) {
            if (use_flash.read_quad(address, read_quad).success()) {
                if (id == read_quad[Index_of_id]) {
                    if (Sentinel_value == read_quad[Index_of_sentinel]) {
                        value = static_cast<int32_t>(read_quad[Index_of_value]);
                        result = Success_result;
                    } else {
                        printf("Search: Illegal sentinel at 0x%08X\r\n", address);
                        result = Error_result;
                    }
                } else {
                    address += U575xG_page_cache::Quadword_width_32;
                }
            } else {
                result = Error_result;
            }
        }
        return Status_code(Success_result == result);
    }

    void U575xG_persistent_storage::assert_id(const uint32_t id, uint8_t &result) {
        if (!Repository::Attribute_type::is_valid_id(id)) {
            cnt_param_error++;
            printf("Illegal u-item id: 0x%08X\r\n", id);
            result = Error_result;
        }
    }

    void U575xG_persistent_storage::dump() const {
    }

    void U575xG_persistent_storage::print_diag() const {
        printf("Store: page=%s, addr=0x%08X, diag=0x%X, left=%d, right=%d, swap=%d\r\n",
               is_left_current ? "left" : "right", current_page_address(), get_diag_code(),
               cnt_left_pages, cnt_right_pages, cnt_swap_pages);
        printf("Cache: void=%d, used=%d, free=%d, fail=%d --> same=%d, clear=%d, write=%d\r\n",
               the_cache.get_void_cnt(), the_cache.get_used_cnt(), the_cache.get_free_cnt(), the_cache.get_fail_cnt(),
               the_cache.get_same_cnt(), the_cache.get_clear_cnt(), the_cache.get_write_cnt());
        printf("Count: copy=%d, update=%d, void=%d, same=%d, write=%d, lookup=%d\r\n",
               cnt_copy, cnt_update, cnt_void, cnt_same, cnt_write, cnt_lookup);
        printf("Error: param=%d, update=%d, void=%d, write=%d, page=%d, copy=%d, erase=%d\r\n",
               cnt_param_error, cnt_update_error, cnt_void_error, cnt_write_error,
               cnt_page_error, cnt_copy_error, cnt_erase_error);
        use_flash.print_diag();
    }
} // STM32U575RG
