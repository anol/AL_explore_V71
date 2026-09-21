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
 * \date   IDEAS/13.12.2019/anolsen
 * \brief
 */

module;
#include <cstdint>
#include <cstdio>

module Utility.Json_printer;

static uint32_t json_level = 0;
static uint32_t json_index[JSON_printer::JSON_max_levels] = {};

void JSON_printer::begin() {
    json_level = 0;
    json_index[json_level] = 0;
    printf("\r\n{");
}

void JSON_printer::end() {
    printf("}\r\n");
}

void JSON_printer::newline() {
    printf("\r\n");
}

void JSON_printer::object_begin(const char *name) {
    if (json_level < JSON_max_levels - 1) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level++]++;
        json_index[json_level] = 0;
        if (name == nullptr) {
            printf("{ ");
        } else {
            printf("\"%s\":{", name);
        }
    }
}

void JSON_printer::object_end() {
    printf("}");
    if ((json_level > 0) && (json_level < JSON_max_levels)) {
        json_index[json_level--]--;
    }
}

void JSON_printer::array_begin(const char *name) {
    if (json_level < JSON_max_levels - 1) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level++]++;
        json_index[json_level] = 0;
        if (name == nullptr) {
            printf("\"%s\":[", "X");
        } else {
            printf("\"%s\":[", name);
        }
    }
}

void JSON_printer::array_end() {
    printf("]");
    if ((json_level > 0) && (json_level < JSON_max_levels)) {
        json_index[json_level--]--;
    }
}

void JSON_printer::bool_value(const char *name, bool value) {
    if (json_level < JSON_max_levels) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level]++;
        if (name) {
            printf("\"%s\":", name);
        }
        printf(" %s", value ? "true" : "false");
    }
}

void JSON_printer::unsigned_value(const char *name, unsigned int value) {
    if (json_level < JSON_max_levels) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level]++;
        if (name) {
            printf("\"%s\":", name);
        }
        printf(" %u", value);
    }
}

void JSON_printer::integer_value(const char *name, int value) {
    if (json_level < JSON_max_levels) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level]++;
        if (name) {
            printf("\"%s\":", name);
        }
        printf(" %d", value);
    }
}

void JSON_printer::float_value(const char *name, float value) {
    if (json_level < JSON_max_levels) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level]++;
        if (name) {
            printf("\"%s\":", name);
        }
        printf(" %d.%03d\r\n", static_cast<int>(value), static_cast<int>(value * 1000) % 1000);
    }
}

void JSON_printer::double_value(const char *name, double value) {
    if (json_level < JSON_max_levels) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level]++;
        if (name) {
            printf("\"%s\":", name);
        }
        printf(" %d.%03d\r\n", static_cast<int>(value), static_cast<int>(value * 1000) % 1000);
    }
}

void JSON_printer::string_value(const char *name, const char *value) {
    if (json_level < JSON_max_levels) {
        if (json_index[json_level] > 0)printf(", ");
        json_index[json_level]++;
        if (name) {
            printf("\"%s\":", name);
        }
        printf(" \"%s\"", value);
    }
}
