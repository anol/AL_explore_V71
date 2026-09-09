target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        SpectraNode_command_empty_help.h
        SpectraNode_command_help.h
        SpectraNode_command_lookup.cpp
        SpectraNode_command_lookup.h
        SpectraNode_command_version.h
        SpectraNode_error_code.h
#        SpectraNode_error_code_src.h
        SpectraNode_keyword.html
        SpectraNode_keyword_lookup.cpp
        SpectraNode_keyword_lookup.h
        SpectraNode_keyword_version.h
        SpectraNode_provider_indication.cpp
        SpectraNode_provider_indication.h
#        SpectraNode_structure.cpp
#        SpectraNode_structure.h
#        SpectraNode_structure_version.h
)
