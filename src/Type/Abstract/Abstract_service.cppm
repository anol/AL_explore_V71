module;

export module Type.Abstract_service;

namespace Abstract
{
    export class Abstract_service
    {
    public:
        virtual ~Abstract_service() = default;
        virtual void initialize() =0;
    };
} // Abstract
