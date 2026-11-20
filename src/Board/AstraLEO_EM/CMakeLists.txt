target_sources(${THE_TARGET_NAME} PUBLIC
        AstraLEO_EM_board.cpp
        AstraLEO_EM_pin_table.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET board_astraleo_em_modules TYPE CXX_MODULES BASE_DIRS . FILES
        AstraLEO_EM_board.cppm
        AstraLEO_EM_pin_table.cppm
)
