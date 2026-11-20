target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET samv71_usart_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SamV71_USART-Config.cppm
        SamV71_USART-Driver.cppm
        SamV71_USART-ISR.cppm
        SamV71_USART.cppm
)
