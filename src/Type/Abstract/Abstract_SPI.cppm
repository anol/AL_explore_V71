export module Type.Abstract_SPI;

export import Type.Abstract_request;

namespace Abstract {
    export class Abstract_SPI {
    public:
        virtual ~Abstract_SPI() = default;

        virtual void initialize() = 0;

        /**
          * @name transfer
          * @param request A pointer to a transfer reqeust instance.
          * @return true=success, false=failed.
          **/
        virtual bool transfer(Abstract_request *request) = 0;
    };
} // Abstract
