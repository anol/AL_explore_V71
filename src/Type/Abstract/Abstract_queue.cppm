module;

#include <cstdint>

export module Type.Abstract_queue;

namespace Abstract {
    export template<typename T>
    class Abstract_queue {
    public:
        virtual ~Abstract_queue() = default;

        virtual bool send(T) = 0;

        virtual bool receive(T *) = 0;

        virtual bool ISR_send(T, uint32_t &context_switch) = 0;

        virtual bool ISR_receive(T *, uint32_t &context_switch) = 0;
    };
} // Abstract
