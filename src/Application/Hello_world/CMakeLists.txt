target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Request_router.cpp
        Request_router.h
        Console_task.cpp
        Console_task.h
        Default_configuration.h
        Hello_world.cpp
        Hello_world.h
        Persistent_parameter_id.h
)
