target_sources(${THE_TARGET_NAME} PUBLIC
        SpectraNode_command_lookup.cpp
        SpectraNode_keyword.html
        SpectraNode_keyword_lookup.cpp
        SpectraNode_provider_indication.cpp
#        SpectraNode_structure.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET domain_generated_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SpectraNode_command_empty_help.cppm
        SpectraNode_command_help.cppm
        SpectraNode_command_lookup.cppm
        SpectraNode_command_table.cppm
        SpectraNode_command_version.cppm
        SpectraNode_keyword_lookup.cppm
        SpectraNode_keyword_version.cppm
        SpectraNode_provider_indication.cppm
#        SpectraNode_structure.cppm
#        SpectraNode_structure_version.cppm
)
