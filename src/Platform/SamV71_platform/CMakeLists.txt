target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Diagnostic.cpp
        SamV71_clock.cpp
        SamV71_clock.h
#        SamV71_ethernet.cpp
#        SamV71_ethernet.h
        SamV71_IO_pin.cpp
        SamV71_IO_pin.h
 #        SamV71_timer.cpp
#        SamV71_timer.h
        SamV71_USART1.cpp
        SamV71_USART1.h
#        SamV71_watchdog.cpp
#        SamV71_watchdog.h
)

add_subdirectory(SAMV71Q21B ${CMAKE_BINARY_DIR}/SAMV71Q21B)
add_subdirectory(${SOURCE_ROOT_DIR}/Platform/Common_platform ${CMAKE_BINARY_DIR}/Common_platform)

