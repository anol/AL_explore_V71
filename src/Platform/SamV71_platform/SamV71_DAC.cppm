module;

#include <cstdint>

export module Platform.SamV71_DAC;
import Type.Abstract_DAC;
import Type.Abstract_DAC;

export namespace SamV71 {
    class SamV71_DAC : public Abstract::Abstract_DAC {
    public:
        SamV71_DAC();

        void initialize() override;

        bool set(uint32_t value) override;
    };
} // SamV71
