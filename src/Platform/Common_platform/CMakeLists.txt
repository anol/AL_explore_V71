target_sources(${THE_TARGET_NAME} PUBLIC
        GCC_syscalls.cpp
        Common_stdio.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET platform_common_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Common_stdio.cppm
        Diagnostic.cppm
)
