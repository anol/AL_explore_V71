module;
#include <cstdint>

export module Platform.Mockup_ADC;
import Type.Abstract_ADC;

export namespace Mockup {
    /// ADC that returns a settable raw value (default: mid-scale of a 14-bit converter, left-aligned by 8).
    class Mockup_ADC : public Abstract::Abstract_ADC {
        enum : uint32_t { Default_value = 8192u << 8 };
        uint32_t the_value{Default_value};

    public:
        void initialize() override {
        }

        bool get(uint32_t &value) override {
            value = the_value;
            return true;
        }

        void set_value(const uint32_t value) { the_value = value; }
    };
} // Mockup
