target_sources(${THE_TARGET_NAME} PUBLIC
        Hello_AstraLEO.cpp
        Service_provider.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET application_hello_astraleo_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Hello_AstraLEO.cppm
        Service_provider.cppm
        SpectraNode_console.cppm
)
add_subdirectory(Dictionary)
add_subdirectory(Provider)
add_subdirectory(Support)
