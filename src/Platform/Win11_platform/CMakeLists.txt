target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
        ${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS/portable/MSVC-MingW
)

# FreeRTOS Windows simulator port: every task is a Windows thread, the tick is a Windows timer thread.
target_sources(${THE_TARGET_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS/portable/MSVC-MingW/port.c
        ${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS/portable/MSVC-MingW/portmacro.h
)
target_link_libraries(${THE_TARGET_NAME} winmm)

# Peripherals are simulated by mockups.
target_sources(${THE_TARGET_NAME} PUBLIC
        Mockup_host.cpp
        Mockup_host.h
        Mockup_SPI.cpp
        Mockup_stdio.cpp
        Mockup_UART.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_win11_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Mockup_ADC.cppm
        Mockup_DAC.cppm
        Mockup_IO_pin.cppm
        Mockup_SPI.cppm
        Mockup_UART.cppm
)

# Not built by Win_hello_world (kept for the on-hold targets):
#   Mockup_ethernet.*, Win11_ethernet.* (needs pcap), Win11_binary_file.*, Win11_embedded_flash.*,
#   Win11_print_support.*

# Host-independent parts of the common platform (the embedded stdio/syscall retargeting is replaced by
# Mockup_stdio.cpp).
set(COMMON_PLATFORM_DIR ${SOURCE_ROOT_DIR}/Platform/Common_platform)
target_sources(${THE_TARGET_NAME} PUBLIC
        ${COMMON_PLATFORM_DIR}/Diagnostic.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_common_modules TYPE CXX_MODULES BASE_DIRS ${COMMON_PLATFORM_DIR} FILES
        ${COMMON_PLATFORM_DIR}/Common_stdio.cppm
        ${COMMON_PLATFORM_DIR}/Diagnostic.cppm
)
