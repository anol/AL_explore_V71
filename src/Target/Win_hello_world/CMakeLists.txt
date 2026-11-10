target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)

target_sources(${THE_TARGET_NAME} PUBLIC
        FreeRTOS_hooks.cpp
        FreeRTOSConfig.h
        Win_hello_world.cpp
        Win_hello_world.h
)

configure_file(Target_config.h.i ${PROJECT_BINARY_DIR}/Target_config.h)

# C o m p i l e r   a n d   l i n k e r   f l a g s
target_link_libraries(${THE_TARGET_NAME} wsock32 ws2_32)

target_compile_options(${THE_TARGET_NAME} PUBLIC
        -O0
        -g3
        -Wall
        -Wextra
        -fmessage-length=0
)

target_compile_definitions(${THE_TARGET_NAME} PUBLIC
        _WIN32_WINNT=0x0601
)

target_link_options(${THE_TARGET_NAME} PUBLIC
        -Xlinker -Map=${PROJECT_NAME}.map
        # Not -static: the CLion 2026.2 MinGW cannot link a fully static image (unresolved
        # __ms_vsnprintf / __intrinsic_setjmpex). Link the GCC runtimes statically instead.
        -static-libgcc
        -static-libstdc++
)

# The only non-system DLL dependency; copy it next to the executable so it runs outside the IDE.
get_filename_component(_MINGW_BIN ${CMAKE_CXX_COMPILER} DIRECTORY)
if (CMAKE_RUNTIME_OUTPUT_DIRECTORY AND EXISTS ${_MINGW_BIN}/libwinpthread-1.dll)
    file(COPY ${_MINGW_BIN}/libwinpthread-1.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif ()
