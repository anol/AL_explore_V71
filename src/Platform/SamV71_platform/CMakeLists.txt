target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        GCC_syscalls.cpp
        SamV71_ethernet.cpp
        SamV71_ethernet.h
        SamV71_stdio.h
        SamV71_stdio.cpp
)
