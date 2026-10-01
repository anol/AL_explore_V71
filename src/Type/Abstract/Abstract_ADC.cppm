module;

export module Type.Abstract_ADC;
#include <cstdint>

export namespace Abstract {
     class Abstract_ADC {
    public:
        virtual ~Abstract_ADC() = default;

        virtual void initialize() = 0;

        virtual bool get(uint32_t& value) = 0;
    };
} // Abstract
