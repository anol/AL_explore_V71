/*
* Copyright (C) 2024 Integrated Detector Electronics AS
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
*
*/
//
// Created by anolsen on 20.02.2020.
//

module;
#include <cstdint>

module Support.CLI_tokenizer;

CLI_tokenizer::CLI_tokenizer(Argument *arguments, const uint32_t max_number_of_arguments) : argument_buffer(arguments),
    use_max_number_of_arguments(max_number_of_arguments) {
}

const char *CLI_tokenizer::tokenize(const char *line, int *p_argc) const {
    uint32_t arg_counter = 0;
    Argument *arg_value = argument_buffer;
    if (line != nullptr) {
        if (*line == '#') {
            line = skip_comment(line);
        } else {
            int argument_size;
            line = skip_control(line);
            while (arg_counter < use_max_number_of_arguments) {
                if (is_AT_mode) {
                    line = copy_AT_argument(arg_value, line, argument_size);
                } else {
                    line = copy_argument(arg_value, line, argument_size);
                }
                if (argument_size > 0) {
                    arg_counter++;
                    arg_value++;
                } else {
                    break;
                }
            }
        }
    }
    *p_argc = arg_counter;
    return line;
}

const char *CLI_tokenizer::copy_argument(Argument *arg_value, const char *line, int &argument_size) {
    char *text = arg_value->text;
    argument_size = 0;
    line = skip_delimiter(line);
    while ((*line > ' ') && *line != ';' && (argument_size < Argument::Max_argument_size)) {
        argument_size++;
        *text++ = *line++;
    }
    *text = 0;
    return line;
}

const char *CLI_tokenizer::copy_AT_argument(Argument *arg_value, const char *line, int &argument_size) {
    char *text = arg_value->text;
    argument_size = 0;
    line = skip_AT_delimiter(line);
    while ((*line > ' ') && *line != ';' && *line != '=' && *line != '_' && *line != ','
           && (argument_size < Argument::Max_argument_size)) {
        argument_size++;
        *text++ = *line++;
    }
    *text = 0;
    return line;
}

const char *CLI_tokenizer::skip_delimiter(const char *line) {
    while (*line == ' ' || *line == ',') line++;
    return line;
}

const char *CLI_tokenizer::skip_AT_delimiter(const char *line) {
    while (*line == ' ' || *line == ',' || *line == '=' || *line == '_') line++;
    return line;
}

const char *CLI_tokenizer::skip_control(const char *line) {
    while ((*line > 0 && *line < ' ') || *line == ';') line++;
    return line;
}

const char *CLI_tokenizer::skip_comment(const char *line) {
    line++;
    while (*line >= ' ') line++;
    return line;
}
