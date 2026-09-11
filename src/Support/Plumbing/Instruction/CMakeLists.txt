target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Instruction_lookup.h
        Instruction_major.cpp
        Instruction_major.h
        Instruction_token.cpp
        Instruction_token.h
        Instruction_utility.cpp
        Instruction_utility.h
        Token_type.h
)
