target_sources(${THE_TARGET_NAME} PUBLIC
        Detector.cpp
        Hello_world.cpp
        Primary_task.cpp
        Service_provider.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET application_hello_world_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Configuration_store.cppm
        Default_configuration.cppm
        Detector.cppm
        Hello_world.cppm
        Primary_task.cppm
        Service_provider.cppm
        SpectraNode_console.cppm
)
