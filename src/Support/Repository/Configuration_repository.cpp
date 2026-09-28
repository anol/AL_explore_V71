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
* @file   Configuration_repository.cpp
* @author AndersEmilOlsen, IDEAS
* @date   02.02.2026
* @brief  
*/

module;
#include <cstdint>
#include <cstdio>

module Support.Configuration_repository;

namespace Repository {
    Configuration_repository::Configuration_repository(const Abstract_configuration &default_config,
                                                       Persistent_storage &store)
        : use_store(store), use_default_config(default_config) {
        the_current_config.clean();
    }

    void Configuration_repository::initialize() {
        auto success{true};
        the_current_config.clean();
        for (uint32_t index = 0; index < use_default_config.get_count(); index++) {
            auto &attribute = use_default_config.get_attribute(index);
            if (auto *name = attribute.get_name()) {
                if (!the_current_config.define_attribute(
                    attribute.get_id(), name, State_default, attribute.get_value()).success()) {
                    success = false;
                }
            }
        }
        if (!success) {
            printf("Failed to initialize the default configuration.");
        }
        if (!use_store.initialize().success()) {
            printf("Failed to initialize the persistent storage.");
        }
    }

    Status_code Configuration_repository::clean() {
        const auto success = use_store.clean();
        initialize();
        return success;
    }

    Status_code Configuration_repository::set(const uint32_t id, const int32_t value) {
        auto success = the_current_config.update_attribute(id, State_changed, value);
        if (!success.success()) { the_set_error++; }
        return success;
    }

    Status_code Configuration_repository::get(const uint32_t id, int32_t &value) {
        auto success = the_current_config.get_value(id, value);
        if (!success.success()) { the_get_error++; }
        return success;
    }

    Status_code Configuration_repository::load() {
        const auto success = use_store.open_reading();
        if (success.success()) {
            for (uint32_t index = 0; index < Current_configuration::get_max_count(); index++) {
                auto &attribute = the_current_config.get_attribute(index);
                if (attribute.is_valid()) {
                    if (int32_t value; use_store.read(attribute.get_id(), value).success()) {
                        attribute.set_value(value, State_loaded);
                    }
                }
            }
        }
        return success;
    }

    Status_code Configuration_repository::save() {
        const auto dirty_count = the_current_config.get_dirty_count();
        auto success = use_store.open_writing(dirty_count).success();
        if (success) {
            // Copy changes from the Current Configuration to the Write Cache.
            for (uint32_t index = 0; success & (index < Current_configuration::get_max_count()); index++) {
                auto &attribute = the_current_config.get_attribute(index);
                if (attribute.is_dirty()) {
                    success = use_store.write_cache(attribute.get_id(), attribute.get_value()).success();
                    attribute.set_state(success ? State_cached : State_write_failed);
                }
            }
        }
        if (success) {
            // Store the changes from the Write Cache to the Flash.
            success = use_store.program_flash().success();
            const auto new_state = success ? State_saved : State_flash_failed;
            for (uint32_t index = 0; index < Current_configuration::get_max_count(); index++) {
                auto &attribute = the_current_config.get_attribute(index);
                if (attribute.is_state(State_cached)) {
                    attribute.set_state(new_state);
                }
            }
        }
        return Status_code(success);
    }

    void Configuration_repository::dump(const char *title) const {
        printf("%s\r\n", title);
        the_current_config.dump();
    }

    void Configuration_repository::print_diag() const {
        printf("Repo: get_error=%d, set_error=%d\r\n", the_get_error, the_set_error);
        use_store.print_diag();
    }
} // Repository
