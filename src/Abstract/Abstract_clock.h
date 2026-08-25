#pragma once

#include <cstdint>

#include <Utility/Utility_types.h>

namespace Abstract {
    class Abstract_clock {
    public:
        Abstract_clock() = default;

        virtual ~Abstract_clock() = default;

        virtual void initialize() = 0;

        virtual uint32_t get_milliseconds() = 0;
    };
}
