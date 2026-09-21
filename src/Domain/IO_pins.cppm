
export module Domain.IO_pins;
import Type.Abstract_IO_pin;

export namespace Domain {
    enum Pin_id : Abstract::Abstract_pin_id {
        Pin_not_used,

        Pin_SDA,
        Pin_SCL,
        Pin_ASIC_RESET_I,
        Pin_ASIC_HOLD_I,
        Pin_ASIC_CLKENEXT,
        Pin_ASIC_TOT_EN,
        Pin_SPI_CS_ASIC5,
        Pin_SPI_CS_ASIC4,
        Pin_SPI_CS_ASIC3,
        Pin_SPI_CS_ASIC2,
        Pin_SPI_CS_ASIC1,
        Pin_SPI_CS_DAC1,
        Pin_SPI_CS_DAC0,
        Pin_FPGA_CS_N,
        Pin_FPGA_IRQ,
        Pin_FPGA_CMD,
        Pin_FPGA_BUSY,
        Pin_RMII_CLKOUT,
        Pin_RMII_TX_EN,
        Pin_RMII_TXD0,
        Pin_RMII_TXD1,
        Pin_RMII_CRSDV,
        Pin_RMII_RXD0,
        Pin_RMII_RXD1,
        Pin_RMII_MDC,
        Pin_RMII_MDIO,
        Pin_SPI_MISO,
        Pin_SPI_MOSI,
        Pin_SPI_SCK,
        Pin_EOUT_MON,
        Pin_UART_RXD1,
        Pin_UART_TXD1,
        Pin_LED0,
        Pin_LED1,
        Pin_ALT_WKUP6,
        Pin_TEMP_ALERT,

        Number_of_pins
    };
} // Application_configuration
