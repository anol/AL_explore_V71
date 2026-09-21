module;

#include <cstdint>

export module Type.Abstract_clock;

namespace Abstract {
    export class Abstract_clock {
    public:
        Abstract_clock() = default;

        virtual ~Abstract_clock() = default;

        virtual void initialize() = 0;

        [[nodiscard]] virtual uint32_t get_milliseconds() const = 0;
    };
}
