target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Abstract_provider.h
)
add_subdirectory(CLI_parser ${CMAKE_BINARY_DIR}/CLI_parser)
add_subdirectory(Instruction ${CMAKE_BINARY_DIR}/Instruction)
