//
// Created by aeols on 2026-11-11.
//

module;

#include <cstdint>

export module Platform.SamV71_ADC;
import :Define;
import :Driver;
import :Handler;

import Type.Abstract_ADC;

export namespace SamV71
{
    class SamV71_ADC : public Abstract::Abstract_ADC
    {
    public:
        enum Channel_number : uint8_t
        {
            AFEC_CHANNEL_0 = 0,
            AFEC_CHANNEL_1,
            AFEC_CHANNEL_2,
            AFEC_CHANNEL_3,
            AFEC_CHANNEL_4,
            AFEC_CHANNEL_5,
            AFEC_CHANNEL_6,
            AFEC_CHANNEL_7,
            AFEC_CHANNEL_8,
            AFEC_CHANNEL_9,
            AFEC_CHANNEL_10,
            AFEC_TEMPERATURE_SENSOR,
        };

    private:
        const uint8_t the_id;
        const Channel_number the_channel;
        SamV71_ADC_driver the_driver;
        SamV71_ADC_handler the_handler;

    public:
        SamV71_ADC() : SamV71_ADC(0, AFEC_TEMPERATURE_SENSOR)
        {
        }

        SamV71_ADC(const uint8_t id, const Channel_number channel)
            : the_id(id), the_channel(channel),
              the_driver(SamV71_ADC_define::get_definition(id)),
              the_handler(SamV71_ADC_define::get_definition(id), the_driver)
        {
        }

        void initialize() override;

        /// Blocking single conversion of the configured channel; value is the raw 12-bit count.
        bool get(uint32_t& value) override;

        void print_diagnostics() const { the_driver.print_diagnostics(); }

        [[nodiscard]] uint8_t get_id() const { return the_id; }
    };

    void SamV71_ADC::initialize()
    {
        the_driver.enable_clock();
        the_driver.disable_ADC();
        the_driver.setup_ADC_registers();
        the_driver.configure_channel(the_channel);
    }

    bool SamV71_ADC::get(uint32_t& value)
    {
        SamV71_ADC_request request{the_channel};
        if (the_driver.convert(&request))
        {
            value = request.get_value();
            return true;
        }
        return false;
    }
} // SamV71
