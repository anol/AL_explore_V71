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
* @file   Abstract_attribute_types.h
* @author AndersEmilOlsen, IDEAS
* @date   02.02.2026
* @brief  
*/

module;
#include <cstdint>

export module Support.Abstract_configuration;
export import Support.Attribute_type;

export namespace Repository {
    class Abstract_configuration {
    public:
        virtual ~Abstract_configuration() = default;

        [[nodiscard]] virtual uint32_t get_count() const = 0;

        [[nodiscard]] virtual const Attribute_type &get_attribute(uint32_t index) const = 0;
    };
} // Repository
