module;

#include <cstdint>

#include "sam.h"
#include "component/afec.h"

export module Platform.SamV71_ADC;
import Type.Abstract_ADC;

export namespace SamV71 {
    class SamV71_ADC : public Abstract::Abstract_ADC {
    public:
        enum Channel_number {
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
            AFEC_CHANNEL_ALL = 0x0FFF,
        };

    private:
        enum {
            peripheral_id = ID_AFEC0,
        };

        afec_registers_t *optional_base {AFEC0_REGS};

    public:
        SamV71_ADC() = default;

        void initialize() override;

        bool get(uint32_t &value) override;

    private:
        void configure_AFEC();

        void configure_channel(Channel_number channel, uint16_t channel_offset);

        void configure_temp_sensor();

        void set_trigger(uint32_t trigger);
    };
} // SamV71
