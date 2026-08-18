target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Abstract_ethernet.h
        Board_full_interface.h
        Clock_interface.h
        IO_pin_interface.h
        SPI_interface.h
        Timer_interface.h
        UART_interface.h
)
