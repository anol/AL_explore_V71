target_sources(${THE_TARGET_NAME} PUBLIC
        Detector.cpp
        Hello_world.cpp
        Service_provider.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET application_hello_world_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Detector.cppm
        Hello_world.cppm
        Service_provider.cppm
        SpectraNode_console.cppm
)
add_subdirectory(Dictionary)
add_subdirectory(Provider)
add_subdirectory(Support)
