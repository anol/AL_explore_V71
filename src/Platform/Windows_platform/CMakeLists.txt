target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Win11_pin_manager.cpp
        Win11_pin_manager.h
        Win11_UART.cpp
        Win11_UART.h
)
