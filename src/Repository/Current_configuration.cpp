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
* @file   Current_configuration.cpp
* @author AndersEmilOlsen, IDEAS
* @date   02.02.2026
* @brief  
*/

#include <cstdio>


#include "Current_configuration.h"

#include <cstring>


namespace Repository {
    void Current_configuration::clean() {
        for (auto &attribute: the_attributes) {
            attribute.clean();
        }
    }

    bool Current_configuration::define_attribute(
        const uint32_t id, const char *name, const Attribute_state state, const int32_t value) {
        if (Attribute_type::is_valid_id(id)) {
            for (auto &attribute: the_attributes) {
                if (!attribute.is_valid()) {
                    attribute.set(id, state, sizeof(value), value, name);
                    return true;
                }
            }
        }
        return false;
    }

    bool Current_configuration::update_attribute(
        const uint32_t id, const Attribute_state state, const int32_t value) {
        if (Attribute_type::is_valid_id(id)) {
            for (auto &attribute: the_attributes) {
                if (attribute.is_id(id)) {
                    if (attribute.get_value() != value) {
                        attribute.set_value(value, state);
                    }
                    return true;
                }
            }
        }
        return false;
    }

    Attribute_type &Current_configuration::get_attribute(const uint32_t index) {
        if (index < Max_attribute_count) { return the_attributes[index]; }
        return the_attributes[0];
    }

    void Current_configuration::set_state(const uint32_t id, const Attribute_state state) {
        if (Attribute_type::is_valid_id(id)) {
            for (auto &attribute: the_attributes) {
                if (attribute.is_id(id)) {
                    attribute.set_state(state);
                    return;
                }
            }
        }
    }

    Attribute_state Current_configuration::get_state(const uint32_t id) const {
        if (Attribute_type::is_valid_id(id)) {
            for (auto &attribute: the_attributes) {
                if (attribute.is_id(id)) {
                    return attribute.get_state();
                }
            }
        }
        return {};
    }

    bool Current_configuration::get_value(const uint32_t id, int32_t &value) const {
        auto success{false};
        if (Attribute_type::is_valid_id(id)) {
            for (auto &attribute: the_attributes) {
                success = attribute.is_id(id);
                if (success) {
                    value = attribute.get_value();
                    break;
                }
            }
        }
        return success;
    }

    int Current_configuration::get_dirty_count() {
        int count{};
        for (const auto &attribute: the_attributes) {
            if (attribute.is_dirty()) {
                count++;
            }
        }
        return count;
    }

    void Current_configuration::dump() const {
        for (const auto &attribute: the_attributes) {
            if (attribute.is_valid()) {
                printf("%2d: %12s = %8d (0x%08X) %s\r\n",
                       attribute.get_id(),
                       attribute.get_state_string(),
                       attribute.get_value(), attribute.get_value(),
                       attribute.get_name());
            }
        }
    }
} // Repository
