module;
#include <cstdint>

export module Type.Abstract_clock;

namespace Abstract {
    export class Abstract_clock {
    public:
        Abstract_clock() = default;

        virtual ~Abstract_clock() = default;

        virtual void initialize() = 0;

        virtual uint32_t get_milliseconds() = 0;
    };
}
