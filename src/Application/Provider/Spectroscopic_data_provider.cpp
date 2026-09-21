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
* @file   Spectroscopic_data_provider.cpp
* @author AndersEmilOlsen, IDEAS
* @date   21.05.2026
* @brief  
*/


#include "Spectroscopic_data_provider.h"

#include "../../Component/IDE3380/Histogram_storage.h"
#include <cstdio>
import Support.Configuration_repository;


void Spectroscopic_data_provider::v_FORMAT_R6(Instruction_major &instruction) {
    the_format = Application::Format_simple_R6;
    if (use_repository.set(Application::Format_demo, the_format).success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Spectroscopic_data_provider::v_FORMAT_CPS(Instruction_major &instruction) {
    the_format = Application::Format_only_CPS;
    if (use_repository.set(Application::Format_demo, the_format).success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Spectroscopic_data_provider::v_FORMAT_N42(Instruction_major &instruction) {
    the_format = Application::Format_complex_N42;
    if (use_repository.set(Application::Format_demo, the_format).success()) {
        instruction.print_ack();
    } else {
        instruction.print_nack(use_repository.get_diag_code());
    }
}

void Spectroscopic_data_provider::v_TIME_date_time(Instruction_major &, const int date_1, const int time_2) {
    printf("%s %s=%d, %s=%d\r\n", "Spectroscopic_data_provider::v_TIME_date_time:",
           "date_1=", date_1, "time_2=", time_2);
}

void Spectroscopic_data_provider::v_GET(Instruction_major &instruction) {
    if (use_histogram.send_data(the_format, the_channel).success()) {
        instruction.print_ack();
    } else {
        printf("Failed to send data: format=%d, channel=%d\r\n", the_format, the_channel);
        instruction.print_nack(99);
    }
}

void Spectroscopic_data_provider::v_GET_RESET(Instruction_major &instruction) {
    if (use_histogram.send_data(the_format, the_channel).success()) {
        use_histogram.clear_histogram_buffer();
        instruction.print_ack();
    } else {
        printf("Failed to send data: format=%d, channel=%d\r\n", the_format, the_channel);
        instruction.print_nack(99);
    }
}

void Spectroscopic_data_provider::send_science_data(const uint32_t cadence) const {
    if (use_histogram.send_data(the_format, the_channel, cadence).success()) {
        use_histogram.clear_histogram_buffer();
    } else {
        printf("!");
    }
}

const char *Spectroscopic_data_provider::format_to_text(const Application::Data_format format) {
    switch (format) {
        case Application::Format_no_data: return "NON";
        case Application::Format_legacy_live_view: return "LEGACY";
        case Application::Format_simple_R6: return "R6";
        case Application::Format_only_CPS: return "CPS";
        case Application::Format_complex_N42: return "N42";
        default: return "?";
    }
}

void Spectroscopic_data_provider::print_status() const {
    switch (the_channel) {
        case 0:
            printf(", channel=%d (inhibit), format=%s", the_channel, format_to_text(the_format));
            break;
        case 17:
            printf(", channel=%d (ana sum), format=%s", the_channel, format_to_text(the_format));
            break;
        case 18:
            printf(", channel=%d (dig sum), format=%s", the_channel, format_to_text(the_format));
            break;
        default:
            printf(", channel=%d (input), format=%s", the_channel, format_to_text(the_format));
            break;
    }
}
