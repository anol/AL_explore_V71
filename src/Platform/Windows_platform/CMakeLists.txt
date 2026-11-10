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

# Peripherals are simulated by the mockup platform.
add_subdirectory(${SOURCE_ROOT_DIR}/Platform/Mockup_platform ${CMAKE_BINARY_DIR}/Mockup_platform)
