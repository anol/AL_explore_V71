target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Mockup_host.cpp
        Mockup_host.h
        Mockup_SPI.cpp
        Mockup_stdio.cpp
        Mockup_UART.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_mockup_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Mockup_ADC.cppm
        Mockup_DAC.cppm
        Mockup_IO_pin.cppm
        Mockup_SPI.cppm
        Mockup_UART.cppm
)

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
