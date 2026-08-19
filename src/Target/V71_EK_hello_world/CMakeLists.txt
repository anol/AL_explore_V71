target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)

target_sources(${THE_TARGET_NAME} PUBLIC
        V71_EK_hello_world.cpp
        V71_EK_hello_world.h
)

# C o m p i l e r   a n d   l i n k e r   f l a g s

set(SAMV71_DIR "${CMAKE_SOURCE_DIR}/src_3rd/Microchip_SAMV71_DFP")
set(CMSIS_DIR "${CMAKE_SOURCE_DIR}/src_3rd/CMSIS/Core")

target_sources(${THE_TARGET_NAME} PUBLIC
        ${SAMV71_DIR}/gcc/gcc/startup_samv71q21b.c
)

target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
        ${SAMV71_DIR}
        ${SAMV71_DIR}/include
        ${CMSIS_DIR}/Include
)

target_link_options(${THE_TARGET_NAME} PUBLIC
        -T ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.ld
)

