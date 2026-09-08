cmake_minimum_required(VERSION 4.3)
set(CMAKE_CXX_STANDARD 20)
set(SOURCE_ROOT_DIR ${CMAKE_SOURCE_DIR}/src)

include(${SOURCE_ROOT_DIR}/Platform/${PLATFORM}_platform/${PLATFORM}_toolchain.cmake)

project(${TARGET})

if (${PLATFORM} STREQUAL "SamV71")
    set(THE_TARGET_NAME "${PROJECT_NAME}.elf")
else ()
    set(THE_TARGET_NAME ${PROJECT_NAME})
endif ()

message("++ Application: ${APP}")
message("++ Board:       ${BOARD}")
message("++ Platform:    ${PLATFORM}")
message("++ Target:      ${TARGET}")
message("++ Executable:  ${THE_TARGET_NAME}")

add_executable(${THE_TARGET_NAME}
)

include(${SOURCE_ROOT_DIR}/Platform/${PLATFORM}_platform/${PLATFORM}_build_options.cmake)

add_subdirectory(${SOURCE_ROOT_DIR}/Application/${APP} Application/${APP})
add_subdirectory(${SOURCE_ROOT_DIR}/Target/${TARGET} ${CMAKE_BINARY_DIR}/Target/${TARGET})
add_subdirectory(${SOURCE_ROOT_DIR}/Board/${BOARD} ${CMAKE_BINARY_DIR}/Board/${BOARD})
add_subdirectory(${SOURCE_ROOT_DIR}/Platform/${PLATFORM}_platform ${CMAKE_BINARY_DIR}/Platform/${PLATFORM})

add_subdirectory(${SOURCE_ROOT_DIR}/Abstract ${CMAKE_BINARY_DIR}/Abstract)
add_subdirectory(${SOURCE_ROOT_DIR}/Dictionary ${CMAKE_BINARY_DIR}/Dictionary)
add_subdirectory(${SOURCE_ROOT_DIR}/Utility ${CMAKE_BINARY_DIR}/Utility)

add_subdirectory(${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS ${CMAKE_BINARY_DIR}/FreeRTOS)
