target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Bias_ADC.cpp
        Bias_ADC.h
        Bias_calibration.cpp
        Bias_calibration.h
        Bias_DAC.cpp
        Bias_DAC.h
        Calibration_analyzer.cpp
        Calibration_analyzer.h
)
