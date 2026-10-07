module;

export module Type.Abstract_DAC;
#include <cstdint>

export namespace Abstract {
    class Abstract_DAC {
    public:
        virtual ~Abstract_DAC() = default;

        virtual void initialize() = 0;

        virtual bool set(uint32_t value) = 0;

        [[nodiscard]] virtual uint32_t get() const = 0;
    };
} // Abstract
