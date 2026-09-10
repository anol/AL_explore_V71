target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Console_receive_task.cpp
        Console_receive_task.h
        Console_service.cpp
        Console_service.h
        Console_transmit_task.cpp
        Console_transmit_task.h
)
