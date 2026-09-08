target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
        ${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS/portable/GCC/ARM_CM7/r0p1/
)
target_sources(${THE_TARGET_NAME} PUBLIC
        startup_samv71q21b.cpp
        ${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS/portable/GCC/ARM_CM7/r0p1/port.c
        ${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS/portable/GCC/ARM_CM7/r0p1/portmacro.h
)

