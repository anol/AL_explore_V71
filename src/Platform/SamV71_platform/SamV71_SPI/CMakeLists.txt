target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET samv71_spi_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SamV71_SPI-Define.cppm
        SamV71_SPI-Driver.cppm
        SamV71_SPI-Handler.cppm
        SamV71_SPI.cppm
)
