cmake_minimum_required(VERSION 4.3)
set(CMAKE_CXX_STANDARD 20)
set(SOURCE_ROOT_DIR ${CMAKE_SOURCE_DIR}/src)

include(${SOURCE_ROOT_DIR}/Platform/${PLATFORM}_platform/${PLATFORM}_toolchain.cmake)

# Get the repository and the current working branch
execute_process(
        COMMAND git config --get remote.origin.url
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REPO
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(IDE_SW_VERSION "1.1.0")

project(${TARGET} VERSION ${IDE_SW_VERSION})

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

target_include_directories(${THE_TARGET_NAME} PRIVATE
        ${PROJECT_BINARY_DIR}
)

include(${SOURCE_ROOT_DIR}/Platform/${PLATFORM}_platform/${PLATFORM}_build_options.cmake)

add_subdirectory(${SOURCE_ROOT_DIR}/Application/${APP} Application/${APP})
add_subdirectory(${SOURCE_ROOT_DIR}/Application/Provider)
add_subdirectory(${SOURCE_ROOT_DIR}/Application/Support)
add_subdirectory(${SOURCE_ROOT_DIR}/Board/${BOARD} ${CMAKE_BINARY_DIR}/Board/${BOARD})
add_subdirectory(${SOURCE_ROOT_DIR}/Component)
add_subdirectory(${SOURCE_ROOT_DIR}/Domain)
add_subdirectory(${SOURCE_ROOT_DIR}/Platform/${PLATFORM}_platform ${CMAKE_BINARY_DIR}/Platform/${PLATFORM})
add_subdirectory(${SOURCE_ROOT_DIR}/Support)
add_subdirectory(${SOURCE_ROOT_DIR}/Target/${TARGET} ${CMAKE_BINARY_DIR}/Target/${TARGET})
add_subdirectory(${SOURCE_ROOT_DIR}/Type)
add_subdirectory(${SOURCE_ROOT_DIR}/Utility)

add_subdirectory(${CMAKE_SOURCE_DIR}/src_3rd/FreeRTOS ${CMAKE_BINARY_DIR}/FreeRTOS)
