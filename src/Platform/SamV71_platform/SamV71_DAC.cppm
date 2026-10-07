module;

#include <cstdint>

export module Platform.SamV71_DAC;
import Type.Abstract_DAC;
import Type.Abstract_DAC;

export namespace SamV71 {
    class SamV71_DAC : public Abstract::Abstract_DAC {
        void *optional_definition{};
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
