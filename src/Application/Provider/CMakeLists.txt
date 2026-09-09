target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Configuration_manager_provider.cpp
        Configuration_manager_provider.h
        Housekeeping_provider.cpp
        Housekeeping_provider.h
        Instrument_calibration_provider.cpp
        Instrument_calibration_provider.h
        Mode_control_provider.cpp
        Mode_control_provider.h
        Spectroscopic_data_provider.cpp
        Spectroscopic_data_provider.h
)
