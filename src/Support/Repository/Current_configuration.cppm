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
* @file   Current_configuration.h
* @author AndersEmilOlsen, IDEAS
* @date   02.02.2026
* @brief  
*/

module;
#include <cstdint>

export module Support.Current_configuration;
export import Support.Attribute_type;
export import Type.Status_code;

export namespace Repository {
    class Current_configuration {
        enum { Max_attribute_count = 128 };

        Attribute_type the_attributes[Max_attribute_count]{};

    public:
        Current_configuration() = default;

        void clean();

        [[nodiscard]] Status_code update_attribute(uint32_t id, Attribute_state state, int32_t value);

        [[nodiscard]] Status_code define_attribute(uint32_t id, const char *name, Attribute_state state, int32_t value);

        [[nodiscard]] Attribute_type &get_attribute(uint32_t index);

        void set_state(uint32_t id, Attribute_state state);

        [[nodiscard]] Attribute_state get_state(uint32_t id) const;

        [[nodiscard]] Status_code get_value(uint32_t id, int32_t& value) const;

        [[nodiscard]] int get_dirty_count();

        static uint32_t get_max_count() { return Max_attribute_count; };

        void dump() const;

    private:
    };
} // Repository
