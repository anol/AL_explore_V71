cmake_minimum_required(VERSION 3.14)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING TRUE)
SET(CMAKE_C_COMPILER_WORKS TRUE)
SET(CMAKE_C++_COMPILER_WORKS TRUE)
set(CMAKE_c++_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS --specs=nosys.specs)

project(uart_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/App)
include_directories(src/Board/SAMV71_XLTRA)
include_directories(src/Board/SAMV71Q21B/include)
include_directories(src/Board/SAMV71Q21B/peripherals/clock)
include_directories(src/Board/SAMV71Q21B/peripherals/gpio)
include_directories(src/Board/SAMV71Q21B/peripherals/pmc)
include_directories(src/Board/SAMV71Q21B/peripherals/uart)
include_directories(src/Labio)
include_directories(src/Utility)

add_executable(uart_test
        src/App/App.cpp
        src/App/App.h
        src/App/Consol.cpp
        src/App/Consol.h
        src/App/Labio.cpp
        src/App/Labio.h
        src/Board/SAMV71_XLTRA/board_definitions.h
        src/Board/SAMV71_XLTRA/board_initialization.cpp
        src/Board/SAMV71_XLTRA/board_initialization.h
        src/Board/SAMV71_XLTRA/cortex_m4_definitions.h
        src/Board/SAMV71_XLTRA/gcc_syscalls.c
        src/Board/SAMV71_XLTRA/interrupt_vectors.c
        src/Board/SAMV71_XLTRA/isr_reset.cpp
        src/Board/SAMV71_XLTRA/isr_reset.h
        src/Board/SAMV71_XLTRA/isr_serial.cpp
        src/Board/SAMV71_XLTRA/isr_serial.h
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
        src/Board/SAMV71Q21B/peripherals/clock/conf_clock.h
        src/Board/SAMV71Q21B/peripherals/clock/genclk.h
        src/Board/SAMV71Q21B/peripherals/clock/osc_driver.c
        src/Board/SAMV71Q21B/peripherals/clock/osc_driver.h
        src/Board/SAMV71Q21B/peripherals/clock/pll.h
        src/Board/SAMV71Q21B/peripherals/clock/sysclk.c
        src/Board/SAMV71Q21B/peripherals/clock/sysclk.h
        src/Board/SAMV71Q21B/peripherals/gpio/GPIO.cpp
        src/Board/SAMV71Q21B/peripherals/gpio/GPIO.h
        src/Board/SAMV71Q21B/peripherals/gpio/hal_gpio.h
        src/Board/SAMV71Q21B/peripherals/gpio/hpl_gpio.h
        src/Board/SAMV71Q21B/peripherals/gpio/hpl_gpio_base.h
        src/Board/SAMV71Q21B/peripherals/gpio/hri_pio_v71b.h
        src/Board/SAMV71Q21B/peripherals/gpio/ioport.c
        src/Board/SAMV71Q21B/peripherals/gpio/ioport.h
        src/Board/SAMV71Q21B/peripherals/gpio/pio_driver.c
        src/Board/SAMV71Q21B/peripherals/gpio/pio_driver.h
        src/Board/SAMV71Q21B/peripherals/gpio/pio_handler.c
        src/Board/SAMV71Q21B/peripherals/gpio/pio_handler.h
        src/Board/SAMV71Q21B/peripherals/pmc/pmc_driver.c
        src/Board/SAMV71Q21B/peripherals/pmc/pmc_driver.h
        src/Board/SAMV71Q21B/peripherals/uart/USART_proxy.cpp
        src/Board/SAMV71Q21B/peripherals/uart/USART_proxy.h
        src/Labio/command_parser.cpp
        src/Labio/command_parser.h
        src/Labio/commands.cpp
        src/Labio/commands.h
        src/Labio/IDE3466_defines.h
        src/Labio/other_pins.h
        src/Labio/temporary_mock.cpp
        src/Labio/temporary_mock.h
        src/Utility/Ringbuffer.cpp
        src/Utility/Ringbuffer.h
        src/Utility/status_code.h
        src/main.cpp src/Labio/Labio_commands.cpp src/Labio/Labio_commands.h src/Utility/Assert_utility.h src/Utility/Assert_utility.cpp)


target_compile_options(uart_test PUBLIC
        # Enable all warnings
        -Wall
        # Kernel development mode (-mkernel)
        -fno-exceptions
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        # Override the automatic link-time optimization (lto)
        -fno-lto
        # No dead code elimination (dce) or dead store elimination (dse)
        #        -fno-dce
        #        -fno-dse
        #        -fno-tree-dce
        #        -fno-tree-dse
        # To reduce image size
        -ffunction-sections
        -fdata-sections
        # Target architecture
        #        -mabi=aapcs
        -mthumb
        -march=armv7-m
        #        -march=armv7e-m+fp.dp
        #        -mfloat-abi=hard
        --specs=nano.specs
        # When target outside 64-megabyte addr range
        #        -mlong-calls
)

target_compile_definitions(uart_test PUBLIC -Dscanf=iscanf)
target_compile_definitions(uart_test PUBLIC -Dprintf=iprintf)
target_compile_definitions(uart_test PUBLIC -D__SAMV71Q21B__)
target_compile_definitions(uart_test PUBLIC -DARM_MATH_CM7=true)
target_compile_definitions(uart_test PUBLIC -DBOARD=SAMV71_XPLAINED_ULTRA)

