target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        startup_samv71q21b.cpp
        system_samv71q21b.cpp
)

