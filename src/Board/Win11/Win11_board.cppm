//
// Created by aeols on 12.08.2026.
//

export module Board.Win11_board;
import Type.Abstract_board;
import Board.Win11_pin_table;
import Platform.Mockup_ADC;
import Platform.Mockup_DAC;
import Platform.Mockup_SPI;
import Platform.Mockup_UART;

export namespace Board {
    /// Host board: every peripheral is a mockup, so the application runs unchanged on Windows.
    class Win11_board : public Abstract::Abstract_board {
        Win11_pin_table the_pin_table{};
        Mockup::Mockup_UART the_UART{};
        Mockup::Mockup_SPI the_SPI{};
        Mockup::Mockup_ADC the_ADC{};
        Mockup::Mockup_DAC the_DAC{};

    public:
        void initialize() override;

        void print_diagnostics() override;

        Abstract::Abstract_UART &get_UART() override { return the_UART; }

        Abstract::Abstract_SPI &get_SPI() override { return the_SPI; }

        Abstract::Abstract_ADC &get_ADC() override { return the_ADC; }

        Abstract::Abstract_DAC &get_DAC() override { return the_DAC; }

        Abstract::Abstract_IO_pin &get_pin(const Abstract::Abstract_pin_id id) override {
            return the_pin_table.get_pin(id);
        }

        Mockup::Mockup_UART &get_mockup_UART() { return the_UART; }

        Mockup::Mockup_ADC &get_mockup_ADC() { return the_ADC; }
    };
} // Board
