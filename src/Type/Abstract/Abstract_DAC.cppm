module;

export module Type.Abstract_DAC;
#include <cstdint>

namespace Abstract {
    export class Abstract_DAC {
    public:
        virtual ~Abstract_DAC() = default;

        virtual void initialize() = 0;

        virtual bool set(uint32_t value) = 0;
    };
} // Abstract
