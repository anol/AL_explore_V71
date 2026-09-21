//
// Created by aeols on 2026-09-11.
//

export module Type.Abstract_pin_table;

export import Type.Abstract_IO_pin;
export import Domain.IO_pins;

namespace Abstract
{
    using Pin = Abstract_IO_pin;

    export class Abstract_pin_table
    {
    public:
        virtual ~Abstract_pin_table() = default;
        virtual Pin& get_pin(Domain::Pin_id id) = 0;
        virtual Status_code set_phase(Pin::Pin_phase) = 0;
        virtual Status_code for_each_pin(void (*)(Pin&)) = 0;
        virtual void print_diagnostics() const = 0;
    };
}
