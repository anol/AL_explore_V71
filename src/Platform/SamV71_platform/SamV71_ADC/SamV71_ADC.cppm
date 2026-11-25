//
// Created by aeols on 2026-11-11.
//

module;

#include <cstdint>

export module Platform.SamV71_ADC;
import :Config;
import :Driver;
import :ISR;

import Type.Abstract_ADC;

export namespace SamV71 {
    class SamV71_ADC : public Abstract::Abstract_ADC {
    public:
        enum Channel_number : uint8_t {
            AFEC0_CHANNEL_0 = 0,
            AFEC0_CHANNEL_1,
            AFEC0_CHANNEL_2,
            AFEC0_CHANNEL_3,
            AFEC0_CHANNEL_4,
            AFEC0_CHANNEL_5,
            AFEC0_CHANNEL_6,
            AFEC0_CHANNEL_7,
            AFEC0_CHANNEL_8,
            AFEC0_CHANNEL_9,
            AFEC0_CHANNEL_10,
            AFEC0_TEMPERATURE_SENSOR,
            AFEC1_CHANNEL_0,
            AFEC1_CHANNEL_1,
            AFEC1_CHANNEL_2,
            AFEC1_CHANNEL_3,
            AFEC1_CHANNEL_4,
            AFEC1_CHANNEL_5,
            AFEC1_CHANNEL_6,
            AFEC1_CHANNEL_7,
            AFEC1_CHANNEL_8,
            AFEC1_CHANNEL_9,
            AFEC1_CHANNEL_10,
            AFEC1_CHANNEL_11,
            Number_of_channels
        };

    private:
        const uint8_t the_id;
        const Channel_number the_channel;
        SamV71_ADC_driver the_driver;
        SamV71_ADC_ISR the_ISR;

    public:
        SamV71_ADC() : SamV71_ADC(0, AFEC0_TEMPERATURE_SENSOR) {}

        SamV71_ADC(const uint8_t id, const Channel_number channel)
            : the_id(id), the_channel(channel),
              the_driver(SamV71_ADC_config::get_config(id)),
              the_ISR(SamV71_ADC_config::get_config(id), the_driver) {}

        void initialize() override;

        bool get(uint8_t channel, uint32_t &value) override;

        [[nodiscard]] uint8_t get_number_of_channels() const override { return Number_of_channels; }

        void print_diagnostics() const { the_driver.print_diagnostics(); }

        [[nodiscard]] uint8_t get_id() const { return the_id; }
    };

    void SamV71_ADC::initialize() {
        the_driver.enable_clock();
        the_driver.disable_ADC();
        the_driver.setup_ADC_registers();
        the_driver.configure_channel(the_channel);
    }

    bool SamV71_ADC::get(uint8_t channel, uint32_t &value) {
        SamV71_ADC_request request{the_channel};
        if (the_driver.convert(&request)) {
            value = request.get_value();
            return true;
        }
        return false;
    }
} // SamV71
