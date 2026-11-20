target_sources(${THE_TARGET_NAME} PUBLIC
        Configuration_manager_provider.cpp
        Housekeeping_provider.cpp
        Instrument_calibration_provider.cpp
        Mode_control_provider.cpp
        Science_data_provider.cpp
        Startup_task.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET application_provider_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Configuration_manager_provider.cppm
        Configuration_store.cppm
        Default_configuration.cppm
        Housekeeping_provider.cppm
        Instrument_calibration_provider.cppm
        Mode_control_provider.cppm
        Science_data_provider.cppm
        Startup_task.cppm
)
