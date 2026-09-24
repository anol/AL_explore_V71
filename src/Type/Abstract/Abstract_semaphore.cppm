module;

export module Type.Abstract_semaphore;

namespace Abstract {
    export class Abstract_semaphore {
    public:
        virtual ~Abstract_semaphore() = default;

        virtual bool take() = 0;

        virtual bool give() = 0;

        virtual bool ISR_take(bool& context_switch) = 0;

        virtual bool ISR_give(bool& context_switch) = 0;
    };
} // Abstract
