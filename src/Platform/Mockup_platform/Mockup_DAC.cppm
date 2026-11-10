module;
#include <cstdint>

export module Platform.Mockup_DAC;
import Type.Abstract_DAC;

export namespace Mockup {
    /// DAC that remembers the last value written.
    class Mockup_DAC : public Abstract::Abstract_DAC {
        uint32_t the_value{};

    public:
        void initialize() override {
        }

        bool set(const uint32_t value) override {
            the_value = value;
            return true;
        }

        [[nodiscard]] uint32_t get() const override { return the_value; }
    };
} // Mockup
