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

namespace Dictionary {

    bool report_anomaly(uint32_t code);
    bool report_anomaly(uint32_t code, int level);

    using Error_code_storage = uint32_t;

    enum Common_code : Error_code_storage {
        Success_code = 0,
        Unimplemented_function = static_cast<Error_code_storage>(-1)
    };

    struct Error_code {
        Error_code_storage the_code{Success_code};

        inline Error_code &operator=(Error_code_storage code) {
            the_code = code;
            return *this;
        }

        inline  Error_code_storage operator()() const { return the_code; }

        inline bool operator==(const Error_code &code) const { return the_code == code.code(); }

        inline bool operator!=(const Error_code &code) const { return the_code != code.code(); }

        inline bool operator==(Error_code_storage code) const { return the_code == code; }

        inline bool operator!=(Error_code_storage code) const { return the_code != code; }

        inline bool success() const { return the_code == Success_code; }

        inline bool failed() const { return the_code != Success_code; }

        inline Error_code_storage code() const { return the_code; }

        inline uint32_t module_id() const { return (the_code >> 16); }

        inline uint32_t local_id() const { return (the_code & 0xFFFF); }
    };

}
