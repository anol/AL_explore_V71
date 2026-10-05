module;

#include <cstdint>

export module Platform.SamV71_DAC;
import Type.Abstract_DAC;
import Type.Abstract_DAC;

export namespace SamV71 {
    class SamV71_DAC : public Abstract::Abstract_DAC {
        void *optional_definition{};

    public:
        SamV71_DAC();

        void initialize() override;

        bool set(uint32_t value) override;

    public:
        void ISR();

    private:
        void ISR_ready();
    };
} // SamV71
