target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Command_handler.cpp
        Command_handler.h
        Command_parser.cpp
        Command_parser.h
        Default_configuration.h
        Hello_world.cpp
        Hello_world.h
        Persistent_parameter_id.h
)
