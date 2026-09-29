

module;
#include <cstdint>

export module Board.V71_EK_pin_table;
import Platform.SamV71_IO_pin;
import Domain.IO_pins;
import Type.Abstract_IO_pin;
import Type.Abstract_pin_table;



export namespace SamV71
{
    class V71_EK_pin_table : public Abstract::Abstract_pin_table
    {
        using Tag = Domain::Pin_id;
        using Pin = SamV71_IO_pin;

        Pin the_pin_table[Tag::Number_of_pins]{
            // Pin name, Function, Phase, Port, Pin, Mode, Type, Default strength=0, Default state=low
            {Tag::Pin_not_used},
            {Tag::Pin_SDA},
            {Tag::Pin_SCL},
            {Tag::Pin_ASIC_RESET_I},
            {Tag::Pin_ASIC_HOLD_I},
            {Tag::Pin_ASIC_CLKENEXT},
            {Tag::Pin_ASIC_TOT_EN},
            {Tag::Pin_SPI_CS_ASIC5, "CS_ASIC5", Pin::Start_up, Pin::Port_C, 12, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 17
            {Tag::Pin_SPI_CS_ASIC4, "CS_ASIC4", Pin::Start_up, Pin::Port_C, 13, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 19
            {Tag::Pin_SPI_CS_ASIC3, "CS_ASIC3", Pin::Start_up, Pin::Port_C, 14, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 97
            {Tag::Pin_SPI_CS_ASIC2, "CS_ASIC2", Pin::Start_up, Pin::Port_C, 15, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 18
            {Tag::Pin_SPI_CS_ASIC1, "CS_ASIC1", Pin::Start_up, Pin::Port_C, 16, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 100
            {Tag::Pin_SPI_CS_DAC1, "CS_DAC1", Pin::Start_up, Pin::Port_C, 19, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 117
            {Tag::Pin_SPI_CS_DAC0, "CS_DAC0", Pin::Start_up, Pin::Port_C, 20, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 120
            {Tag::Pin_FPGA_CS_N, "CS_FPGA", Pin::Start_up, Pin::Port_C, 22, Pin::Mode_GPIO, Pin::Out_normal, 0, true}, // 124
            {Tag::Pin_FPGA_IRQ},
            {Tag::Pin_FPGA_CMD},
            {Tag::Pin_FPGA_BUSY},
            {Tag::Pin_RMII_CLKOUT},
            {Tag::Pin_RMII_TX_EN},
            {Tag::Pin_RMII_TXD0},
            {Tag::Pin_RMII_TXD1},
            {Tag::Pin_RMII_CRSDV},
            {Tag::Pin_RMII_RXD0},
            {Tag::Pin_RMII_RXD1},
            {Tag::Pin_RMII_MDC},
            {Tag::Pin_RMII_MDIO},
            {Tag::Pin_SPI_MISO, "SPI0_MISO", Pin::Start_up, Pin::Port_D, 20, Pin::Mode_B, Pin::Input_pull_down}, // 65
            {Tag::Pin_SPI_MOSI, "SPI0_MOSI", Pin::Start_up, Pin::Port_D, 21, Pin::Mode_B, Pin::Input_pull_up}, // 63
            {Tag::Pin_SPI_SCK, "SPI0_SCK", Pin::Start_up, Pin::Port_D, 22, Pin::Mode_B, Pin::Out_normal}, // 60
            {Tag::Pin_EOUT_MON},
            {Tag::Pin_UART_RXD1, "USART1_RXD", Pin::Start_up, Pin::Port_A, 21, Pin::Mode_A, Pin::Input_normal}, // 32
            {Tag::Pin_UART_TXD1, "USART1_TXD", Pin::Start_up, Pin::Port_B, 4, Pin::Mode_D, Pin::Out_normal}, // 105
            {Tag::Pin_LED0, "LED0", Pin::Start_up, Pin::Port_A, 23, Pin::Mode_GPIO, Pin::Out_normal}, // 46
            {Tag::Pin_LED1, "LED1", Pin::Start_up, Pin::Port_C, 9, Pin::Mode_GPIO, Pin::Out_normal}, // 86
            {Tag::Pin_ALT_WKUP6},
            {Tag::Pin_TEMP_ALERT},

        };

    public:
        void initialize() override;
        Abstract::Abstract_IO_pin& get_pin(Abstract::Abstract_pin_id id) override;
        Status_code set_phase(Pin::Pin_phase) override;
        Status_code for_each_pin(void* user, void (*func)(void*, Abstract::Abstract_IO_pin&)) override;
        [[nodiscard]] uint8_t get_pin_count() const override { return Domain::Number_of_pins; };
        void print_diagnostics() const override;

    private:
        static void initialize_matrix();

    };
}
