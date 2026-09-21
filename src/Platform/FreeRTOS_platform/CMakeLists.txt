target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_freertos_modules TYPE CXX_MODULES BASE_DIRS . FILES
        FreeRTOS_queue.cppm
        FreeRTOS_semaphore.cppm
        FreeRTOS_task.cppm
)
