target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Windows_ethernet.cpp
        Windows_ethernet.h
)
