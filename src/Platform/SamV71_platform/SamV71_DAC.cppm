module;

#include <cstdint>

export module Platform.SamV71_DAC;
import Type.Abstract_DAC;
import Type.Abstract_DAC;

namespace SamV71_DAC_config {
    struct Config; // defined in the implementation unit
}

export namespace SamV71 {
    class SamV71_DAC : public Abstract::Abstract_DAC {
        const SamV71_DAC_config::Config *optional_config{};
        uint32_t the_DAC_setting{};

    public:
        SamV71_DAC();

        void initialize() override;

        bool set(uint32_t value) override;

        [[nodiscard]] uint32_t get() const override;

        void ISR();

    private:
        void ISR_ready();
    };
} // SamV71
