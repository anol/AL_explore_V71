/*
 * Copyright (C) 2026 Integrated Detector Electronics AS
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
 * \date   IDEAS/22.03.2021/aeols
 * \brief
 */

#pragma once

#include <cstdint>
#include <cstddef>

#include "Abstract_error.h"

namespace Dictionary
{
    bool report_anomaly(uint32_t code);
    bool report_anomaly(uint32_t code, int level);

    using Error_code_storage = uint32_t;

    enum Common_code : Error_code_storage
    {
        Success_code = 0,
        Unimplemented_function = static_cast<Error_code_storage>(-1)
    };

    class Error_code : public Abstract::Abstract_error
    {
        Error_code_storage the_code{Success_code};

    public:
        Error_code() = default;

        explicit Error_code(const int code) : the_code(code)
        {
        };

        explicit Error_code(const Common_code code) : the_code(code)
        {
        };

        [[nodiscard]] bool success() const override { return the_code == Success_code; }

        [[nodiscard]] bool failed() const override { return the_code != Success_code; }

        Error_code& operator=(Error_code_storage code)
        {
            the_code = code;
            return *this;
        }

        Error_code_storage operator()() const { return the_code; }

        bool operator==(const Error_code& code) const { return the_code == code.code(); }

        bool operator!=(const Error_code& code) const { return the_code != code.code(); }

        bool operator==(Error_code_storage code) const { return the_code == code; }

        bool operator!=(Error_code_storage code) const { return the_code != code; }

        [[nodiscard]] Error_code_storage code() const { return the_code; }

        [[nodiscard]] uint32_t module_id() const { return (the_code >> 16); }

        [[nodiscard]] uint32_t local_id() const { return (the_code & 0xFFFF); }
    };
}
