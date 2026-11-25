module;

export module Type.Abstract_ADC;
#include <cstdint>

export namespace Abstract {
    class Abstract_ADC {
    public:
        virtual ~Abstract_ADC() = default;

        virtual void initialize() = 0;

        virtual bool get(uint8_t channel, uint32_t &value) = 0;

        [[nodiscard]] virtual uint8_t get_number_of_channels() const = 0;
    };
} // Abstract
