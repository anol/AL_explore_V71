target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET samv71_i2c_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SamV71_I2C-Define.cppm
        SamV71_I2C-Driver.cppm
        SamV71_I2C-Handler.cppm
        SamV71_I2C.cppm
)
