target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Abstract_application.h
        Abstract_board.h
        Abstract_clock.h
        Abstract_ethernet.h
        Abstract_IO_pin.h
        Abstract_pin_table.h
        Abstract_service.h
        Abstract_target.h
        Abstract_task.h
        Abstract_timer.h
        Abstract_UART.h
)
