target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Heartbeat_task.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET support_heartbeat_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Heartbeat_task.cppm
)
