target_sources(${THE_TARGET_NAME} PUBLIC
        Configuration_manager_provider.cpp
        Housekeeping_provider.cpp
        Instrument_calibration_provider.cpp
        Mode_control_provider.cpp
        Spectroscopic_data_provider.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET application_provider_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Configuration_manager_provider.cppm
        Housekeeping_provider.cppm
        Instrument_calibration_provider.cppm
        Mode_control_provider.cppm
        Spectroscopic_data_provider.cppm
)
