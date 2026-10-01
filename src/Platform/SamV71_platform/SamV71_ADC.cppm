module;

#include <cstdint>

export module Platform.SamV71_ADC;
import Type.Abstract_ADC;

export namespace SamV71 {
    class SamV71_ADC : public Abstract::Abstract_ADC {
    public:
        SamV71_ADC();

        void initialize() override;

        bool get(uint32_t &value) override;
    };
} // SamV71
