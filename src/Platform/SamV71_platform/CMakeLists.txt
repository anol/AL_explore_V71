# FreeRTOSConfig.h lives here: the RTOS port configuration belongs to the platform.
target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)

target_sources(${THE_TARGET_NAME} PUBLIC
        FreeRTOSConfig.h
        SamV71_clock.cpp
        SamV71_DAC.cpp
        SamV71_IO_pin.cpp
        SamV71_platform.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_samv71_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SamV71_clock.cppm
        SamV71_DAC.cppm
        SamV71_IO_pin.cppm
        SamV71_platform.cppm
)

add_subdirectory(SamV71_ADC ${CMAKE_BINARY_DIR}/SamV71_ADC)
add_subdirectory(SamV71_I2C ${CMAKE_BINARY_DIR}/SamV71_I2C)
add_subdirectory(SamV71_SPI ${CMAKE_BINARY_DIR}/SamV71_SPI)
add_subdirectory(SamV71_USART ${CMAKE_BINARY_DIR}/SamV71_USART)
add_subdirectory(SAMV71Q21B ${CMAKE_BINARY_DIR}/SAMV71Q21B)
add_subdirectory(${SOURCE_ROOT_DIR}/Platform/Common_platform ${CMAKE_BINARY_DIR}/Common_platform)
