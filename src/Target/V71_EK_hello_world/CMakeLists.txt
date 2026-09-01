target_sources(${THE_TARGET_NAME} PUBLIC
        V71_EK_hello_world.cpp
        V71_EK_hello_world.h
)

# C o m p i l e r   a n d   l i n k e r   f l a g s

set(SAMV71_DIR "${CMAKE_SOURCE_DIR}/src_3rd/Microchip")
set(CMSIS_DIR "${CMAKE_SOURCE_DIR}/src_3rd/CMSIS/Core")

target_sources(${THE_TARGET_NAME} PUBLIC
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

### Alternative image format: srec, binary, ihex ###
add_custom_target(${PROJECT_NAME}.hex ALL)

add_dependencies(${PROJECT_NAME}.hex ${THE_TARGET_NAME})

add_custom_command(TARGET ${PROJECT_NAME}.hex POST_BUILD
        COMMAND ${OBJCOPY}
        -O ihex
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${THE_TARGET_NAME}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex
)

