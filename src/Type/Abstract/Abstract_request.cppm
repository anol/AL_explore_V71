module;

export module Type.Abstract_request;
export import Type.Abstract_semaphore;

namespace Abstract
{
    export class Abstract_request
    {
    public:
        virtual ~Abstract_request() = default;
        [[nodiscard]] virtual Abstract_semaphore& get_semaphore() const = 0;
    };
} // Abstract
