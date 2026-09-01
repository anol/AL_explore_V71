cmake_minimum_required(VERSION 3.14)
project(spi_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/App)
include_directories(src/Board/SAMV71_XLTRA)
include_directories(src/Board/SAMV71Q21B/include)
include_directories(src/Board/SAMV71Q21B/peripheral)
include_directories(src/Detector/IDE3466)
include_directories(src/Labio)
include_directories(src/Utility)

add_executable(spi_test
    src/App/App.cpp
    src/App/App.h
        src/App/Consol.cpp
        src/App/Consol.h
        src/App/Command_handler.cpp
        src/App/Command_handler.h
    src/App/Labio.cpp
    src/App/Labio.h
    src/App/New_CLI.cpp
    src/App/New_CLI.h
    src/Board/SAMV71_XLTRA/board_definitions.h
#    src/Board/SAMV71_XLTRA/board_initialization.cpp
    src/Board/SAMV71_XLTRA/board_initialization.h
    src/Board/SAMV71_XLTRA/cortex_m4_definitions.h
#    src/Board/SAMV71_XLTRA/gcc_syscalls.c
    src/Board/SAMV71_XLTRA/interrupt_vectors.c
    src/Board/SAMV71_XLTRA/isr_reset.cpp
    src/Board/SAMV71_XLTRA/isr_reset.h
    src/Board/SAMV71_XLTRA/isr_serial.cpp
    src/Board/SAMV71_XLTRA/isr_serial.h
    src/Board/SAMV71_XLTRA/isr_spi.cpp
    src/Board/SAMV71_XLTRA/isr_spi.h
    src/Board/SAMV71Q21B/include/component/acc.h
    src/Board/SAMV71Q21B/include/component/aes.h
    src/Board/SAMV71Q21B/include/component/afec.h
    src/Board/SAMV71Q21B/include/component/chipid.h
    src/Board/SAMV71Q21B/include/component/dacc.h
    src/Board/SAMV71Q21B/include/component/efc.h
    src/Board/SAMV71Q21B/include/component/gmac.h
    src/Board/SAMV71Q21B/include/component/gpbr.h
    src/Board/SAMV71Q21B/include/component/hsmci.h
    src/Board/SAMV71Q21B/include/component/i2sc.h
    src/Board/SAMV71Q21B/include/component/icm.h
    src/Board/SAMV71Q21B/include/component/isi.h
    src/Board/SAMV71Q21B/include/component/matrix.h
    src/Board/SAMV71Q21B/include/component/mcan.h
    src/Board/SAMV71Q21B/include/component/mlb.h
    src/Board/SAMV71Q21B/include/component/pio.h
    src/Board/SAMV71Q21B/include/component/pmc.h
    src/Board/SAMV71Q21B/include/component/pwm.h
    src/Board/SAMV71Q21B/include/component/qspi.h
    src/Board/SAMV71Q21B/include/component/rstc.h
    src/Board/SAMV71Q21B/include/component/rswdt.h
    src/Board/SAMV71Q21B/include/component/rtc.h
    src/Board/SAMV71Q21B/include/component/rtt.h
    src/Board/SAMV71Q21B/include/component/sdramc.h
    src/Board/SAMV71Q21B/include/component/smc.h
    src/Board/SAMV71Q21B/include/component/spi.h
    src/Board/SAMV71Q21B/include/component/ssc.h
    src/Board/SAMV71Q21B/include/component/supc.h
    src/Board/SAMV71Q21B/include/component/tc.h
    src/Board/SAMV71Q21B/include/component/trng.h
    src/Board/SAMV71Q21B/include/component/twihs.h
    src/Board/SAMV71Q21B/include/component/uart.h
    src/Board/SAMV71Q21B/include/component/usart.h
    src/Board/SAMV71Q21B/include/component/usbhs.h
    src/Board/SAMV71Q21B/include/component/utmi.h
    src/Board/SAMV71Q21B/include/component/wdt.h
    src/Board/SAMV71Q21B/include/component/xdmac.h
    src/Board/SAMV71Q21B/include/instance/acc.h
    src/Board/SAMV71Q21B/include/instance/aes.h
    src/Board/SAMV71Q21B/include/instance/afec0.h
    src/Board/SAMV71Q21B/include/instance/afec1.h
    src/Board/SAMV71Q21B/include/instance/chipid.h
    src/Board/SAMV71Q21B/include/instance/dacc.h
    src/Board/SAMV71Q21B/include/instance/efc.h
    src/Board/SAMV71Q21B/include/instance/gmac.h
    src/Board/SAMV71Q21B/include/instance/gpbr.h
    src/Board/SAMV71Q21B/include/instance/hsmci.h
    src/Board/SAMV71Q21B/include/instance/i2sc0.h
    src/Board/SAMV71Q21B/include/instance/i2sc1.h
    src/Board/SAMV71Q21B/include/instance/icm.h
    src/Board/SAMV71Q21B/include/instance/isi.h
    src/Board/SAMV71Q21B/include/instance/matrix.h
    src/Board/SAMV71Q21B/include/instance/mcan0.h
    src/Board/SAMV71Q21B/include/instance/mcan1.h
    src/Board/SAMV71Q21B/include/instance/mlb.h
    src/Board/SAMV71Q21B/include/instance/pioa.h
    src/Board/SAMV71Q21B/include/instance/piob.h
    src/Board/SAMV71Q21B/include/instance/pioc.h
    src/Board/SAMV71Q21B/include/instance/piod.h
    src/Board/SAMV71Q21B/include/instance/pioe.h
    src/Board/SAMV71Q21B/include/instance/pmc.h
    src/Board/SAMV71Q21B/include/instance/pwm0.h
    src/Board/SAMV71Q21B/include/instance/pwm1.h
    src/Board/SAMV71Q21B/include/instance/qspi.h
    src/Board/SAMV71Q21B/include/instance/rstc.h
    src/Board/SAMV71Q21B/include/instance/rswdt.h
    src/Board/SAMV71Q21B/include/instance/rtc.h
    src/Board/SAMV71Q21B/include/instance/rtt.h
    src/Board/SAMV71Q21B/include/instance/sdramc.h
    src/Board/SAMV71Q21B/include/instance/smc.h
    src/Board/SAMV71Q21B/include/instance/spi0.h
    src/Board/SAMV71Q21B/include/instance/spi1.h
    src/Board/SAMV71Q21B/include/instance/ssc.h
    src/Board/SAMV71Q21B/include/instance/supc.h
    src/Board/SAMV71Q21B/include/instance/tc0.h
    src/Board/SAMV71Q21B/include/instance/tc1.h
    src/Board/SAMV71Q21B/include/instance/tc2.h
    src/Board/SAMV71Q21B/include/instance/tc3.h
    src/Board/SAMV71Q21B/include/instance/trng.h
    src/Board/SAMV71Q21B/include/instance/twihs0.h
    src/Board/SAMV71Q21B/include/instance/twihs1.h
    src/Board/SAMV71Q21B/include/instance/twihs2.h
    src/Board/SAMV71Q21B/include/instance/uart0.h
    src/Board/SAMV71Q21B/include/instance/uart1.h
    src/Board/SAMV71Q21B/include/instance/uart2.h
    src/Board/SAMV71Q21B/include/instance/uart3.h
    src/Board/SAMV71Q21B/include/instance/uart4.h
    src/Board/SAMV71Q21B/include/instance/usart0.h
    src/Board/SAMV71Q21B/include/instance/usart1.h
    src/Board/SAMV71Q21B/include/instance/usart2.h
    src/Board/SAMV71Q21B/include/instance/usbhs.h
    src/Board/SAMV71Q21B/include/instance/utmi.h
    src/Board/SAMV71Q21B/include/instance/wdt.h
    src/Board/SAMV71Q21B/include/instance/xdmac.h
    src/Board/SAMV71Q21B/include/nvic_helpers.h
    src/Board/SAMV71Q21B/include/samv71q21b.h
    src/Board/SAMV71Q21B/include/samv71q21b_pio.h
    src/Board/SAMV71Q21B/peripheral/clock/conf_clock.h
    src/Board/SAMV71Q21B/peripheral/clock/genclk.h
    src/Board/SAMV71Q21B/peripheral/clock/osc_driver.c
    src/Board/SAMV71Q21B/peripheral/clock/osc_driver.h
    src/Board/SAMV71Q21B/peripheral/clock/pll.h
    src/Board/SAMV71Q21B/peripheral/clock/sysclk.c
    src/Board/SAMV71Q21B/peripheral/clock/sysclk.h
    src/Board/SAMV71Q21B/peripheral/gpio/GPIO_pin.cpp
    src/Board/SAMV71Q21B/peripheral/gpio/GPIO_pin.h
#    src/Board/SAMV71Q21B/peripheral/pmc/pmc_driver.c
    src/Board/SAMV71Q21B/peripheral/pmc/pmc_driver.h
    src/Board/SAMV71Q21B/peripheral/spi/SPI_proxy.cpp
    src/Board/SAMV71Q21B/peripheral/spi/SPI_proxy.h
    src/Board/SAMV71Q21B/peripheral/uart/USART_proxy.cpp
    src/Board/SAMV71Q21B/peripheral/uart/USART_proxy.h
    src/Detector/IDE3466/IDE3466.cpp
    src/Detector/IDE3466/IDE3466.h
    src/Detector/IDE3466/IDE3466_analog_readout.cpp
    src/Detector/IDE3466/IDE3466_analog_readout.h
    src/Detector/IDE3466/IDE3466_calibration.cpp
    src/Detector/IDE3466/IDE3466_calibration.h
    src/Detector/IDE3466/IDE3466_digital_readout.cpp
    src/Detector/IDE3466/IDE3466_digital_readout.h
    src/Detector/IDE3466/IDE3466_register.h
        src/Detector/IDE3466/IDE3466_register_bank.cpp
        src/Detector/IDE3466/IDE3466_register_bank.h
    src/Labio/command_parser.cpp
    src/Labio/command_parser.h
    src/Labio/commands.cpp
    src/Labio/commands.h
    src/Labio/IDE3466_defines.h
    src/Labio/other_pins.h
#    src/Utility/Assert_utility.cpp
    src/Utility/Assert_utility.h
    src/Utility/Ringbuffer.cpp
    src/Utility/Ringbuffer.h
    src/Utility/Size_adapter.cpp
    src/Utility/Size_adapter.h
    src/Utility/status_code.h
    src/main.cpp src/App/Command_handler.cpp src/App/Command_handler.h)

#target_compile_options(spi_test PUBLIC -mtune=cortex-m7)
#target_compile_options(spi_test PUBLIC -march=armv7-m)
target_compile_definitions(spi_test PUBLIC -Dscanf=iscanf)
target_compile_definitions(spi_test PUBLIC -Dprintf=iprintf)
target_compile_definitions(spi_test PUBLIC -D__SAMV71Q21B__)
target_compile_definitions(spi_test PUBLIC -DARM_MATH_CM7=true)
target_compile_definitions(spi_test PUBLIC -DBOARD=SAMV71_XPLAINED_ULTRA)

