module;

export module Type.Abstract_queue;

namespace Abstract {
    export template<typename T>
    class Abstract_queue {
    public:
        virtual ~Abstract_queue() = default;

        virtual bool send(T) = 0;

        virtual bool receive(T *) = 0;

        virtual bool ISR_send(T) = 0;

        virtual bool ISR_receive(T *) = 0;
    };
} // Abstract
