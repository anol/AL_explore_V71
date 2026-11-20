target_sources(${THE_TARGET_NAME} PUBLIC
        AstraLEO_command_lookup.cpp
        AstraLEO_provider_indication.cpp
        AstraLEO_structure.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET domain_generated_modules TYPE CXX_MODULES BASE_DIRS . FILES
        AstraLEO_command_empty_help.cppm
        AstraLEO_command_help.cppm
        AstraLEO_command_lookup.cppm
        AstraLEO_command_table.cppm
        AstraLEO_command_version.cppm
        AstraLEO_provider_indication.cppm
        AstraLEO_structure.cppm
        AstraLEO_structure_version.cppm
)
