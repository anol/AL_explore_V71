//
// Created by aeols on 12.08.2026.
//

module;
#include <cstdint>

export module Board.V71_EK_board;
import Platform.Common_stdio;
import Platform.SamV71_clock;
import Platform.SamV71_SPI;
import Platform.SamV71_USART;
import Type.Abstract_board;
import Board.V71_EK_pin_table;
import Domain.IO_pins;


export namespace Board
{
    using namespace SamV71;
    using namespace Abstract;

    class V71_EK_board : public Abstract_board
    {
        SamV71_clock the_clock{};
        V71_EK_pin_table the_pin_table{};
        SamV71_SPI the_SPI{0, the_pin_table.get_pin(Domain::Pin_SPI_CS_ASIC2)};
        SamV71_USART the_UART{1};
        Platform::Common_stdio the_console{&the_UART};

    public:
        V71_EK_board() = default;

        void initialize() override;

        void print_diagnostics() override;

        Abstract_UART& get_UART() override { return the_UART; }
        Abstract_SPI& get_SPI() override { return the_SPI; }
        Abstract_IO_pin& get_pin(const Abstract::Abstract_pin_id id) override { return the_pin_table.get_pin(id); }

    private:
        static void enable_cache();
    };
} // Board
