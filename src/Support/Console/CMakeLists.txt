target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Console_receive_task.cpp
        Console_transmit_task.cpp
        Console_service.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET CXX_MODULES BASE_DIRS . FILES
        Console_service.cppm
        Console_transmit_task.cppm
        Console_receive_task.cppm
)
