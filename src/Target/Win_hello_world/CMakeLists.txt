target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)

target_sources(${THE_TARGET_NAME} PUBLIC
        Win_hello_world.cpp
        Win_hello_world.h
)

# C o n f i g u r a t i o n   f i l e
configure_file(${PROJECT_NAME}_config.h.in ${PROJECT_BINARY_DIR}/Target_config.h)

# C o m p i l e r   a n d   l i n k e r   f l a g s
target_link_libraries(${THE_TARGET_NAME} wsock32 ws2_32)

target_compile_options(${THE_TARGET_NAME} PUBLIC
        -O0
        -g3
        -Wall
        -Wextra
        -fmessage-length=0
        -Wcast-qual
        -static-libgcc
        -static-libstdc++
)

add_compile_definitions(_WIN32_WINNT=0x0601)

target_link_options(${THE_TARGET_NAME} PUBLIC
        -Xlinker -Map=${PROJECT_NAME}.map
        -static
        #        -static-libgcc
        #        -static-libstdc++
)

#target_link_libraries(${THE_TARGET_NAME}
#        mingw32
#        mingwex
#        msvcrt
#        gcc
#        gcc_eh
#        pthread
#        wsock32
#        ws2_32
#)
