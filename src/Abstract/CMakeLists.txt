target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Abstract_ethernet.h
        Board_full_interface.h
        Clock_interface.h
        Abstract_IO_pin.h
        Timer_interface.h
        UART_interface.h
)
