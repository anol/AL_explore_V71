module;
#include <cstdint>

export module Platform.Mockup_DAC;
import Type.Abstract_DAC;

export namespace Mockup {
    /// 12-bit DAC that remembers the last value written; rejects out-of-range values like SamV71_DAC.
    class Mockup_DAC : public Abstract::Abstract_DAC {
        enum : uint32_t { Max_value = (1u << 12) - 1 };
        uint32_t the_value{};

    public:
        void initialize() override {
        }

        bool set(const uint32_t value) override {
            if (value > Max_value) {
                return false;
            }
            the_value = value;
            return true;
        }

        [[nodiscard]] uint32_t get() const override { return the_value; }
    };
} // Mockup
