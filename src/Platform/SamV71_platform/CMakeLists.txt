target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        GCC_syscalls.cpp
        SamV71_clock.cpp
        SamV71_clock.h
        SamV71_timer.cpp
        SamV71_timer.h
        SamV71_watchdog.cpp
        SamV71_watchdog.h
        SamV71_board_support.cpp
        SamV71_board_support.h
        SamV71_build_options.cmake
        SamV71_ethernet.cpp
        SamV71_ethernet.h
        SamV71_IO_pin.cpp
        SamV71_IO_pin.h
        SamV71_pin_manager.cpp
        SamV71_pin_manager.h
        SamV71_stdio.cpp
        SamV71_stdio.h
)
