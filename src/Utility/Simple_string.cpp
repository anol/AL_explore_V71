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

#include <cstring>
#include "Simple_string.h"


/// Compare up to equal lenght, or wildcard
bool Simple_string::strict_wildcard_match(const char *match_rule, const char *search_string, int max_length) {
    while ((max_length-- > 0) && (*match_rule != /*(const char)*/ Wildcard)) {
        if ((*match_rule == 0) && (*search_string == 0)) {
            return true;
        }
        if ((*match_rule != *search_string)) {
            return false;
        }
        match_rule++;
        search_string++;
    }
    return true;
}

/// Compare up to rule length
bool Simple_string::wildcard_compare(const char *match_rule, const char *search_string, int max_length) {
    while ((max_length-- > 0) && (*match_rule != 0)) {
        if ((*match_rule != /*(const char)*/ Wildcard) && (*match_rule != *search_string)) {
            return false;
        }
        match_rule++;
        search_string++;
    }
    return true;
}

bool Simple_string::stringmask_to_bitmask(const char *string, int32_t &bitmask) {
    bitmask = 0;
    if (string != nullptr) {
        uint32_t lenght = strlen(string);
        if ((lenght > 0) && (lenght < 17)) {        //TODO: Magic number
            const char *next = string;
            for (uint32_t gpc = 0; gpc < lenght; gpc++) {
                char sym = *next++;
                bitmask <<= 2u;
                if (sym == '0') {
                    // Enable anticoincidence
                    bitmask |= 3u;
                } else if (sym == '1') {
                    // Enable coincidence
                    bitmask |= 1u;
                } else if ((sym != 'X') && (sym != 'x')) {
                    bitmask = 0;
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}
