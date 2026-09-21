target_sources(${THE_TARGET_NAME} PUBLIC
        CLI_help.cpp
        CLI_parser.cpp
        CLI_stack.cpp
        CLI_tokenizer.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET support_cli_parser_modules TYPE CXX_MODULES BASE_DIRS . FILES
        CLI_help.cppm
        CLI_parser.cppm
        CLI_stack.cppm
        CLI_tokenizer.cppm
)
