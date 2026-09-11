target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        V71_EK_pin_table.h
        V71_EK_board.cpp
        V71_EK_board.h
        V71_EK_configuration.h
        V71_EK_pin_manager.cpp
        V71_EK_pin_manager.h
)
