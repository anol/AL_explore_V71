target_sources(${THE_TARGET_NAME} PUBLIC
        Bias_ADC.cpp
        Bias_calibration.cpp
        Bias_DAC.cpp
        Calibration_analyzer.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET component_sipm_bias_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Bias_ADC.cppm
        Bias_calibration.cppm
        Bias_DAC.cppm
        Calibration_analyzer.cppm
)
