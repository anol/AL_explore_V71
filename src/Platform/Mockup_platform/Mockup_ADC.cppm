module;
#include <cstdint>

export module Platform.Mockup_ADC;
import Type.Abstract_ADC;
import Type.Abstract_DAC;

export namespace Mockup {
    /**
     * ADC returning a raw 12-bit count, like SamV71_ADC.
     * When constructed with a DAC it is a loopback: it "reads" the DAC's most recent setting
     * (both are 12-bit, so counts map one-to-one). Otherwise it returns a settable value.
     */
    class Mockup_ADC : public Abstract::Abstract_ADC {
        enum : uint32_t { Max_value = (1u << 12) - 1, Default_value = (Max_value + 1) / 2 };
        const Abstract::Abstract_DAC *optional_source{};
        uint32_t the_value{Default_value};

    public:
        Mockup_ADC() = default;

        explicit Mockup_ADC(const Abstract::Abstract_DAC &source) : optional_source(&source) {
        }

        void initialize() override {
        }

        bool get(uint32_t &value) override {
            value = optional_source ? optional_source->get() : the_value;
            if (value > Max_value) {
                value = Max_value;
            }
            return true;
        }

        void set_value(const uint32_t value) { the_value = value; }
    };
} // Mockup
