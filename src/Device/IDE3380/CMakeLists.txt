target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        IDE3380_definitions.h
        IDE3380_interface.cpp
        IDE3380_interface.h
        IDE3380_readout_control.cpp
        IDE3380_readout_control.h
        IDE3380_register_access.cpp
        IDE3380_register_access.h
        IDE3380_register_decoder.cpp
        IDE3380_register_decoder.h
)
