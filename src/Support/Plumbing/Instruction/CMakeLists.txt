target_sources(${THE_TARGET_NAME} PUBLIC
        Instruction_major.cpp
        Instruction_token.cpp
        Instruction_utility.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET support_instruction_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Instruction_lookup.cppm
        Instruction_major.cppm
        Instruction_token.cppm
        Instruction_utility.cppm
        Token_type.cppm
)
