target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        CLI_help.cpp
        CLI_help.h
        CLI_parser.cpp
        CLI_parser.h
        CLI_stack.cpp
        CLI_stack.h
        CLI_tokenizer.cpp
        CLI_tokenizer.h
)
