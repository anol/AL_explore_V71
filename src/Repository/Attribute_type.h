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
* @file   Attribute_type.h
* @author AndersEmilOlsen, IDEAS
* @date   26.02.2026
* @brief  
*/


#ifndef UNIT_TEST_ATTRIBUTE_TYPE_H
#define UNIT_TEST_ATTRIBUTE_TYPE_H

namespace Repository {
    enum Attribute_state {
        State_void, State_default, State_cached, State_saved, State_loaded, State_changed,
        State_write_failed, State_flash_failed,
    };

    class Attribute_type {
        uint32_t the_id;
        uint32_t the_size;
        int32_t the_value;
        const char *the_name;
        Attribute_state the_state;

    public:
        Attribute_type() = default;

        constexpr Attribute_type(const uint32_t id, const Attribute_state state, const uint32_t size,
                                 const int32_t value, const char *const name)
            : the_id(id), the_size(size), the_value(value), the_name(name), the_state(state) {
        }

        void set(const uint32_t id, const Attribute_state state, const uint32_t size,
                 const int32_t value, const char *const name) {
            the_id = id;
            the_size = size;
            the_value = value;
            the_name = name;
            the_state = state;
        }

        void set_state(Attribute_state state) { the_state = state; }

        void set_value(int32_t value, Attribute_state state) {
            the_value = value;
            the_state = state;
        }

        void clean() {
            the_id = 0;
            the_state = State_void;
            the_size = 0;
            the_value = 0;
            the_name = nullptr;
        }

        [[nodiscard]] bool is_id(uint32_t id) const { return the_id == id; }
        [[nodiscard]] bool is_valid() const { return the_id > 0; }
        [[nodiscard]] bool is_state(Attribute_state state) const { return the_state == state; }
        [[nodiscard]] bool is_dirty() const { return State_changed == the_state || State_flash_failed == the_state; }
        [[nodiscard]] uint32_t get_id() const { return the_id; }
        [[nodiscard]] Attribute_state get_state() const { return the_state; }
        [[nodiscard]] uint32_t get_size() const { return the_size; }
        [[nodiscard]] const char *get_name() const { return the_name; }
        [[nodiscard]] int32_t get_value() const { return the_value; }

        [[nodiscard]] const char *get_state_string() const {
            switch (the_state) {
                case State_void: return "void";
                case State_default: return "default";
                case State_cached: return "cached";
                case State_saved: return "saved";
                case State_loaded: return "loaded";
                case State_changed: return "changed";
                case State_write_failed: return "write failed";
                case State_flash_failed: return "flash failed";
                default: return "?";
            }
        }

        static bool is_valid_id(const uint32_t id) { return id > 0x00 && id < 0xFF; }
    };
} // Repository
#endif //UNIT_TEST_ATTRIBUTE_TYPE_H
