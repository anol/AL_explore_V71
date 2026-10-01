module;

export module Type.Abstract_board;
export import Type.Abstract_IO_pin;
export import Type.Abstract_SPI;
export import Type.Abstract_UART;
export import Type.Abstract_ADC;
export import Type.Abstract_DAC;

namespace Abstract {
    export class Abstract_board {
    public:
        Abstract_board() = default;

        virtual ~Abstract_board() = default;

        virtual void initialize() = 0;

        virtual Abstract_UART &get_UART() = 0;

        virtual Abstract_SPI &get_SPI() = 0;

        virtual Abstract_ADC &get_ADC() = 0;

        virtual Abstract_DAC &get_DAC() = 0;

        virtual Abstract_IO_pin &get_pin(Abstract_pin_id id) = 0;

        virtual void print_diagnostics() = 0;
    };
} // Abstract
