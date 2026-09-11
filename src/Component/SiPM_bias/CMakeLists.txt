target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Abstract_calibration_test.h
        Bias_ADC.cpp
        Bias_ADC.h
        Bias_calibration.cpp
        Bias_calibration.h
        Bias_DAC.cpp
        Bias_DAC.h
        Calibration_analyzer.cpp
        Calibration_analyzer.h
        ../IDE3380/IDE3380_test_noise_floor.cpp
        ../IDE3380/IDE3380_test_noise_floor.h
        ../IDE3380/IDE3380_test_pedestal.cpp
        ../IDE3380/IDE3380_test_pedestal.h
#        Sensor_STTS22H.cpp
#        Sensor_STTS22H.h
)
