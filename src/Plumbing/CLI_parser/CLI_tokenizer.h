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

#ifndef TARGET_WINDOWS_CLI_TOKENIZER_H
#define TARGET_WINDOWS_CLI_TOKENIZER_H

#include <cstdint>

class CLI_tokenizer {
public:
    struct Argument {
        enum {
            Max_argument_size = 32
        };

        char text[Max_argument_size];
    };

private:
    Argument *argument_buffer;
    const uint32_t use_max_number_of_arguments;
    bool is_AT_mode{};

public:
    CLI_tokenizer(Argument *arguments, uint32_t max_number_of_arguments);

    void set_mode(bool AT_mode) { is_AT_mode = AT_mode; }

    const char *tokenize(const char *line, int *p_argc) const;

private:
    static const char *skip_comment(const char *line);

    static const char *skip_control(const char *line);

    static const char *skip_delimiter(const char *line);

    static const char *skip_AT_delimiter(const char *line);

    static const char *copy_argument(Argument *arg_value, const char *line, int &argument_size) ;

    static const char *copy_AT_argument(Argument *arg_value, const char *line, int &argument_size) ;
};


#endif //TARGET_WINDOWS_CLI_TOKENIZER_H
