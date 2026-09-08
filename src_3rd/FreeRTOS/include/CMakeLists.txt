target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        atomic.h
        croutine.h
        event_groups.h
        FreeRTOS.h
        list.h
        message_buffer.h
        mpu_prototypes.h
        mpu_syscall_numbers.h
        mpu_wrappers.h
        newlib-freertos.h
        picolibc-freertos.h
        portable.h
        projdefs.h
        queue.h
        semphr.h
        stack_macros.h
        StackMacros.h
        stream_buffer.h
        task.h
        timers.h
)
