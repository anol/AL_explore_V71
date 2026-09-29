target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET type_generic_modules TYPE CXX_MODULES BASE_DIRS . FILES
        SPI_transfer_request.cppm
)
