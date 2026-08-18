//
// Created by anolsen on 13.12.2019.
//

#ifndef UTILITY_JSON_PRINTER_H
#define UTILITY_JSON_PRINTER_H

namespace JSON_printer {
    enum {
        JSON_max_levels = 10,
    };

    void begin();

    void end();

    void newline();

    void object_begin(const char *name);

    void object_end();

    void array_begin(const char *name);

    void array_end();

    void bool_value(const char *name, bool value);

    void unsigned_value(const char *name, unsigned int value);

    void integer_value(const char *name, int value);

    void float_value(const char *name, float value);

    void double_value(const char *name, double value);

    void string_value(const char *name, const char *value);

}

#endif //UTILITY_JSON_PRINTER_H
