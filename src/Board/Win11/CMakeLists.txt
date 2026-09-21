target_sources(${THE_TARGET_NAME} PUBLIC
        Win11_board.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET board_win11_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Win11_board.cppm
)
