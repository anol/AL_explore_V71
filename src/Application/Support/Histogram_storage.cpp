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
* @file   Histogram_storage.cpp
* @author AndersEmilOlsen, IDEAS
* @date   17.03.2026
* @brief  
*/


#include "Histogram_storage.h"

#include <ctime>

#include "Event_counter.h"
#include <stdio.h>
// #include "stm32u5xx_hal.h"
#include "Bias_ADC.h"
#include "Bias_DAC.h"
#include "Configuration_repository.h"

namespace Application
{
    uint32_t cnt_histogram_events{};

    volatile uint32_t Histogram_storage::the_event_count{};

    void Histogram_storage::print_diag()
    {
        printf("Hist: event=%d, channel=%d", the_event_count, cnt_histogram_events);
        for (auto& cnt : cnt_channel_hits)
        {
            printf(", %d", cnt);
        }
        printf("\r\n");
    }

    void Histogram_storage::update_histogram(const TXD_data_t& data)
    {
        cnt_histogram_events++;
        if (data.ADC_Channel < Number_of_channels)
        {
            // 2026-03-24/Aeo: Few entries apper at channel number 0 - collect diagnostics.
            cnt_channel_hits[data.ADC_Channel]++;
        }
        uint16_t channel_index = data.ADC_Channel;
        uint16_t bin_index = data.ADC_value;
        channel_index--; // Change to zero-based
        if (channel_index < Number_of_channels)
        {
            auto* event_cnt = &the_histogram_buffer[channel_index][Diag_bin_event_count];
            *event_cnt = *event_cnt + 1;
            if (data.trigger_flag)
            {
                auto* trig_cnt = &the_histogram_buffer[channel_index][Diag_bin_trigger_count];
                *trig_cnt = *trig_cnt + 1;
            }
        }
        if (channel_index < IDE3380_summing_channel)
        {
            auto offset = the_channel_pedestal[channel_index];
            if (bin_index > offset)
            {
                bin_index -= offset;
                the_digital_sum += bin_index; // Accumulate for use in the "digital summing channel".
                if (bin_index < Histogram_width && (data.trigger_flag || the_disable_trigger_flag))
                {
                    auto* value = &the_histogram_buffer[channel_index][bin_index];
                    *value = *value + 1;
                }
            }
        }
        else if (channel_index == IDE3380_summing_channel)
        {
            count_event();
            if (bin_index < Histogram_width && data.trigger_flag)
            {
                auto* value = &the_histogram_buffer[IDE3380_summing_channel][bin_index];
                *value = *value + 1;
            }
            the_digital_sum = the_digital_sum / IDE3380_input_count;
            if (the_digital_sum < Histogram_width)
            {
                auto* value = &the_histogram_buffer[Digital_summing][the_digital_sum];
                *value = *value + 1;
                auto* event_cnt = &the_histogram_buffer[Digital_summing][Diag_bin_event_count];
                *event_cnt = *event_cnt + 1;
            }
            else
            {
                auto* overflow = &the_histogram_buffer[Digital_summing][Diag_bin_trigger_count];
                *overflow = *overflow + 1;
            }
            the_digital_sum = 0;
        }
    }

    void Histogram_storage::clear_histogram_buffer()
    {
        uint32_t index{};
        for (auto& channel : the_histogram_buffer)
        {
            for (auto& bin : channel)
            {
                bin = 0;
            }
            channel[Diag_bin_channel_number] = index++;
        }
        the_digital_sum = 0;
    }

    void Histogram_storage::clear_isotope_identification()
    {
        for (auto& bin : the_transit_buffer)
        {
            bin = 0;
        }
    }

    bool Histogram_storage::update_seconds_past(uint32_t& cadence)
    {
        bool success{};
        auto timestamp = 1000u /** HAL_GetTick()*/;
        if (the_old_timestamp > 0)
        {
            if (timestamp > the_old_timestamp)
            {
                cadence = timestamp - the_old_timestamp;
                success = true;
            }
        }
        the_old_timestamp = timestamp;
        return success;
    }

    bool Histogram_storage::send_data(const Data_format format, const int channel, uint32_t cadence)
    {
        bool success{channel <= Number_of_channels};
        if (success)
        {
            const uint8_t channel_index = channel - 1;
            switch (format)
            {
            case Format_legacy_live_view:
                if (channel > 0)
                {
                    auto* buffer = get_channel_buffer(channel_index);
                    for (uint32_t i = Skip_diag_info; i < Histogram_width; i++)
                    {
                        auto counts = buffer[i];
                        for (uint32_t j = 0; j < counts; j++)
                        {
                            if (auto keV = i; keV > 0)
                            {
                                printf("%u;", keV);
                            }
                        }
                    }
                    printf("\r\n");
                }
                break;
            case Format_simple_R6:
                if (channel > 0)
                {
                    auto hard = use_event_counter.reset_event_count(); // Hard event counter
                    if (cadence == 0)
                    {
                        if (update_seconds_past(cadence))
                        {
                            printf("R6:events=%d;CH=%d;", hard, channel_index + 1);
                        }
                    }
                    else
                    {
                        printf("R6:CPS=%d;CH=%d;", hard / cadence, channel_index + 1);
                    }
                    auto* buffer = get_channel_buffer(channel_index);
                    for (uint32_t bin = Skip_diag_info; bin < Histogram_width; bin++)
                    {
                        auto count = buffer[bin];
                        if (count > 0)
                        {
                            printf("%d=%u;", bin, count);
                        }
                    }
                    printf("\r\n\r\n");
                }
                break;
            case Format_only_CPS:
                {
                    auto soft = reset_event_count();
                    auto hard = use_event_counter.reset_event_count();
                    auto TORO = use_IDE3380.get_readout_control().reset_TORO_count();
                    auto sense = Calibration::Bias_ADC::get_last_sense();
                    auto bias = Calibration::Bias_DAC::get_last_bias();
                    if (cadence == 0)
                    {
                        if (update_seconds_past(cadence))
                        {
                            printf("Events=%d, ISR=%d, soft=%d, sense=%d, bias=%d\r\n",
                                   (int)hard, (int)TORO, (int)soft, (int)sense, (int)bias);
                        }
                    }
                    else
                    {
                        printf("CPS=%d, ISR=%d, soft=%d, sense=%d, bias=%d\r\n",
                               (int)hard / cadence, (int)TORO / cadence, (int)soft / cadence, (int)sense, (int)bias);
                    }
                    break;
                }
            default:
                {
                    success = false;
                    break;
                }
            }
        }
        return success;
    }

    bool Histogram_storage::update_pedestals(Repository::Configuration_repository& repository, Parameter_id base_offset)
    {
        auto success{true};
        for (uint8_t channel = 0; success && channel < Number_of_inputs; channel++)
        {
            int32_t value;
            auto id = base_offset + channel;
            success = repository.get(id, value);
            if (success)
            {
                set_channel_pedestal(channel, value);
            }
        }
        return success;
    }

    bool Histogram_storage::update_gain(Repository::Configuration_repository& repository, Parameter_id base_gain)
    {
        auto success{true};
        for (uint8_t channel = 0; success && channel < Number_of_inputs; channel++)
        {
            int32_t value;
            auto id = base_gain + channel;
            success = repository.get(id, value);
            if (success)
            {
                set_channel_gain(channel, value);
            }
        }
        return success;
    }
} // Application
