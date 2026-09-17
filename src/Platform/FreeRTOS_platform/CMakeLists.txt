target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        FreeRTOS_queue.h
        FreeRTOS_semaphore.h
)
