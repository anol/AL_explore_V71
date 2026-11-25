module;

export module Board.AstraLEO_EM_board;
import Type.Abstract_board;
import Domain.IO_pins;
import Board.AstraLEO_EM_pin_table;
import Platform.SamV71_ADC;
import Platform.SamV71_clock;
import Platform.SamV71_DAC;
import Platform.SamV71_I2C;
import Platform.SamV71_USART;
import Platform.SamV71_SPI;

export namespace Board {
    using namespace SamV71;
    using namespace Abstract;

    class AstraLEO_EM_board : public Abstract_board {
        SamV71_clock the_clock{};
        AstraLEO_EM_pin_table the_pin_table{};
        SamV71::SamV71_SPI the_SPI{0, the_pin_table.get_pin(Domain::Pin_SPI_CS_ASIC1)};
        SamV71_I2C the_I2C{0}; // TWIHS0 (PA3 = SDA, PA4 = SCL)
        SamV71_USART the_UART{1};
        SamV71_ADC the_ADC{0, SamV71_ADC::AFEC0_CHANNEL_0}; // AFEC0_AD0 (PD30)
        SamV71_DAC the_DAC{};

    public:
        AstraLEO_EM_board() = default;

        void initialize() override;

        void print_diagnostics() override;

        Abstract_UART &get_UART() override { return the_UART; }

        Abstract_SPI &get_SPI() override { return the_SPI; }

        Abstract_I2C &get_I2C() override { return the_I2C; }

        Abstract_ADC &get_ADC() override { return the_ADC; }

        Abstract_DAC &get_DAC() override { return the_DAC; }

        Abstract_IO_pin &get_pin(const Abstract_pin_id id) override { return the_pin_table.get_pin(id); }
    };
} // Board
