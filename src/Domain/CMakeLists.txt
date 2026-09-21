target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Dictionary.h
        Persistent_parameter_id.h
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET domain_modules TYPE CXX_MODULES BASE_DIRS . FILES
        IO_pins.cppm
)
add_subdirectory(Generated_code ${CMAKE_BINARY_DIR}/Generated_code)
