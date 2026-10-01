target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        #        SamV71_ethernet.cpp
        #        SamV71_ethernet.h
        #        SamV71_timer.cpp
        #        SamV71_timer.h
        #        SamV71_watchdog.cpp
        #        SamV71_watchdog.h
        SamV71_ADC.cpp
        SamV71_clock.cpp
        SamV71_DAC.cpp
        SamV71_IO_pin.cpp
        SamV71_SPI.cpp
        SamV71_USART.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_samv71_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SamV71_ADC.cppm
        SamV71_clock.cppm
        SamV71_DAC.cppm
        SamV71_IO_pin.cppm
        SamV71_SPI.cppm
        SamV71_USART.cppm
)

add_subdirectory(SAMV71Q21B ${CMAKE_BINARY_DIR}/SAMV71Q21B)
add_subdirectory(${SOURCE_ROOT_DIR}/Platform/Common_platform ${CMAKE_BINARY_DIR}/Common_platform)
