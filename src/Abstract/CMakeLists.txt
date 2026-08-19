target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Abstract_application.h
        Abstract_board.h
        Abstract_clock.h
        Abstract_ethernet.h
        Abstract_IO_pin.h
        Abstract_target.h
        Abstract_UART.h
)
