cmake_minimum_required(VERSION 4.3)
set(CMAKE_CXX_STANDARD 20)

project(${TARGET})

set(THE_TARGET_NAME ${PROJECT_NAME})
set(SOURCE_ROOT_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(${THE_TARGET_NAME}
)

add_subdirectory(${SOURCE_ROOT_DIR}/Abstract ${CMAKE_BINARY_DIR}/Abstract)
add_subdirectory(${SOURCE_ROOT_DIR}/Target/${TARGET} ${CMAKE_BINARY_DIR}/Target/${TARGET})
add_subdirectory(${SOURCE_ROOT_DIR}/Platform/${PLATFORM} ${CMAKE_BINARY_DIR}/Platform/${PLATFORM})
