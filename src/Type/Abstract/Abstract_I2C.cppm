module;

#include <cstdint>

export module Type.Abstract_I2C;
export import Type.Abstract_request;

namespace Abstract {
    export class Abstract_I2C {
    public:
        virtual ~Abstract_I2C() = default;

        virtual void initialize() = 0;

        virtual bool read(uint8_t device_id, uint8_t data_width, uint32_t &data) = 0;

        virtual bool write(uint8_t device_id, uint8_t data_width, uint32_t data) = 0;
    };
} // Abstract
