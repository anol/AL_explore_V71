/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 * \date   IDEAS/28.10.2020/aeols
 * \brief
 */

module;
#include <cstdint>

export module Utility.Simple_string;


export namespace Simple_string {
    enum : uint8_t {
        Wildcard = 0xFF
    };

    bool strict_wildcard_match(const char *match_rule, const char *search_string, int max_length);

    bool wildcard_compare(const char *match_rule, const char *search_string, int max_length);

    bool stringmask_to_bitmask(const char *string, int32_t &bitmask);

}
