target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
        ${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS/portable/MSVC-MingW
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Win11_pin_manager.cpp
        Win11_pin_manager.h
        Win11_UART.cpp
        Win11_UART.h
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_win11_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Win11_SPI.cppm
)
