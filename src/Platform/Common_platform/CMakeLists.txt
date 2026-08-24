target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        GCC_syscalls.cpp
        Common_stdio.cpp
        Common_stdio.h
)
