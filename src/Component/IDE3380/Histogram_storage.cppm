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
* @file   Histogram_storage.h
* @author AndersEmilOlsen, IDEAS
* @date   17.03.2026
* @brief  
*/

module;
#include <cstdint>
#include <cstring>
#include "Persistent_parameter_id.h"

export module Component.Histogram_storage;
import Component.IDE3380_interface;
import Component.Event_counter;
import Type.Status_code;
import Support.Configuration_repository;



export namespace Application {
    using namespace IDE3380;

    class Histogram_storage {
    public:
        enum {
            Diag_bin_channel_number = 0,
            Diag_bin_event_count = 1,
            Diag_bin_trigger_count = 2,
            Last_input_channel = Input_channel_16 - 1,
            Summing_channel = Analog_summing_ch - 1,
            Digital_summing = Digital_summing_ch - 1,
            Histogram_width = 4096,
            Skip_diag_info = 3,
            Default_channel_offset = 155,
            Default_channel_gain = 1,
        };

    private:
        Event_counter &use_event_counter;
        IDE3380_interface &use_IDE3380;
        volatile uint32_t the_histogram_buffer[Number_of_specters][Histogram_width]{};
        uint32_t the_transit_buffer[Histogram_width]{};
        uint32_t cnt_channel_hits[Number_of_channels]{};
        uint32_t the_channel_pedestal[Number_of_inputs]{Default_channel_offset};
        uint32_t the_channel_gain[Number_of_inputs]{Default_channel_gain};
        uint32_t the_digital_sum{};
        uint32_t the_old_timestamp{};

        static_assert(static_cast<int>(Last_input_channel) == IDE3380_last_channel, "Channel count error");
        static_assert(static_cast<int>(Number_of_channels) == IDE3380_channel_count + 1, "Channel count error");
        static_assert(static_cast<int>(Histogram_width) == static_cast<int>(IDE3380_ADC_range), "Size missmatch");

        bool the_disable_trigger_flag{};
        bool the_check_zero_flag{};

        static volatile uint32_t the_event_count;

    public:
        Histogram_storage(Event_counter &counter, IDE3380_interface &ASIC)
            : use_event_counter(counter), use_IDE3380(ASIC) {
        }

        void update_histogram(const TXD_data_t &data);

        void clear_histogram_buffer();

        void clear_isotope_identification();

        Status_code update_seconds_past(uint32_t &cadence);

        const uint32_t *get_channel_buffer(const uint8_t channel_index) {
            if (channel_index < Number_of_channels) {
                memcpy(the_transit_buffer, (void *) &the_histogram_buffer[channel_index], sizeof(the_transit_buffer));
            }
            return the_transit_buffer;
        }

        Status_code send_data(Data_format, int channel, uint32_t cadence = 0);

        void clear_channel_pedestal() {
            for (auto &pedestal: the_channel_pedestal) {
                pedestal = 0;
            }
        }

        void set_channel_pedestal(int channel, int32_t offset) {
            if (channel < Number_of_inputs) {
                the_channel_pedestal[channel] = offset;
            }
        }

        void set_channel_gain(int channel, int32_t gain) {
            if (channel < Number_of_inputs) {
                the_channel_gain[channel] = gain;
            }
        }

        Status_code update_pedestals(Repository::Configuration_repository &repository, Parameter_id base_offset);

        Status_code update_gain(Repository::Configuration_repository &repository, Parameter_id base_gain);

        static uint8_t get_channel_count() { return Number_of_channels; }

        [[nodiscard]] uint32_t get_entry(const uint8_t channel_index, const uint16_t bin_index) const {
            uint32_t entry{};
            if (channel_index < Number_of_channels && bin_index < Histogram_width) {
                entry = the_histogram_buffer[channel_index][bin_index];
            }
            return entry;
        }

        void check_zero(const bool check) { the_check_zero_flag = check; }

        void disable_trigger_flag(const bool disable) { the_disable_trigger_flag = disable; }

        void print_diag();

        static uint32_t reset_event_count() {
            auto events = the_event_count;
            the_event_count = 0;
            return events;
        };

    private:
        static void count_event() { the_event_count = the_event_count + 1; }
    };
} // Application
