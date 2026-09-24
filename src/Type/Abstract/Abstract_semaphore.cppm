module;

#include <cstdint>

export module Type.Abstract_semaphore;

namespace Abstract {
    export class Abstract_semaphore {
    public:
        virtual ~Abstract_semaphore() = default;

        virtual bool take() = 0;

        virtual bool give() = 0;

        virtual bool ISR_take(uint32_t& context_switch) = 0;

        virtual bool ISR_give(uint32_t& context_switch) = 0;
    };
} // Abstract
