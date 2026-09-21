module;

export module Type.Abstract_application;

namespace Abstract {
    export class Abstract_application {
    public:
        Abstract_application() = default;

        virtual ~Abstract_application() = default;

        virtual void initialize() = 0;

        virtual void run() =0;
    };
} // Abstract
