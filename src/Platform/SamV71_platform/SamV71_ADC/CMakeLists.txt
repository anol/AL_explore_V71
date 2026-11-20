target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET samv71_adc_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SamV71_ADC-Config.cppm
        SamV71_ADC-Driver.cppm
        SamV71_ADC-ISR.cppm
        SamV71_ADC.cppm
)
