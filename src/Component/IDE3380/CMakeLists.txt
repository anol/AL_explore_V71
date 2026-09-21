target_sources(${THE_TARGET_NAME} PUBLIC
        Event_counter.cpp
        Histogram_storage.cpp
        IDE3380_interface.cpp
        IDE3380_readout_control.cpp
        IDE3380_register_access.cpp
        IDE3380_register_decoder.cpp
        IDE3380_test_noise_floor.cpp
        IDE3380_test_pedestal.cpp)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET component_ide3380_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Event_counter.cppm
        Histogram_storage.cppm
        IDE3380_definitions.cppm
        IDE3380_interface.cppm
        IDE3380_readout_control.cppm
        IDE3380_register_access.cppm
        IDE3380_register_decoder.cppm
        IDE3380_test_noise_floor.cppm
        IDE3380_test_pedestal.cppm)
