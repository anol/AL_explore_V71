target_sources(${THE_TARGET_NAME} PUBLIC
        V71_EK_board.cpp
        V71_EK_pin_table.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET board_v71_ek_modules TYPE CXX_MODULES BASE_DIRS . FILES
        V71_EK_board.cppm
        V71_EK_pin_table.cppm
)
