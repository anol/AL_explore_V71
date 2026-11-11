# Build options for the Win11 host platform (MinGW-w64 GCC, FreeRTOS Windows simulator port).

execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
        OUTPUT_VARIABLE ARCH_NAME
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_options(${THE_TARGET_NAME} PUBLIC
        -Wall
        -Wno-format
)

target_compile_definitions(${THE_TARGET_NAME} PUBLIC
        WIN32_MOCKUP=1
)
