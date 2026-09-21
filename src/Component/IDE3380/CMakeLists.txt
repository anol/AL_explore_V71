target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Event_counter.cpp
        Event_counter.h
        Histogram_storage.cpp
        Histogram_storage.h
        IDE3380_definitions.h
        IDE3380_interface.cpp
        IDE3380_interface.h
        IDE3380_readout_control.cpp
        IDE3380_readout_control.h
        IDE3380_register_access.cpp
        IDE3380_register_access.h
        IDE3380_register_decoder.cpp
        IDE3380_register_decoder.h
        IDE3380_test_noise_floor.cpp
        IDE3380_test_noise_floor.h
        IDE3380_test_pedestal.cpp
        IDE3380_test_pedestal.h)
