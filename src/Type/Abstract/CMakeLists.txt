target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET type_abstract_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Abstract_ADC.cppm
        Abstract_application.cppm
        Abstract_board.cppm
        Abstract_clock.cppm
        Abstract_DAC.cppm
        Abstract_ethernet.cppm
        Abstract_IO_pin.cppm
        Abstract_pin_table.cppm
        Abstract_provider.cppm
        Abstract_queue.cppm
        Abstract_request.cppm
        Abstract_scenario.cppm
        Abstract_semaphore.cppm
        Abstract_service.cppm
        Abstract_SPI.cppm
        Abstract_target.cppm
        Abstract_task.cppm
        Abstract_timer.cppm
        Abstract_UART.cppm
)
