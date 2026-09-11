target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Dictionary.h
        IO_pins.h
)
add_subdirectory(Generated_code ${CMAKE_BINARY_DIR}/Generated_code)
