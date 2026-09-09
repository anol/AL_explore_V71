target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Cadence_control.cpp
        Cadence_control.h
        Event_counter.cpp
        Event_counter.h
        gamma_peak_detector.c
        gamma_peak_detector.h
        Histogram_storage.cpp
        Histogram_storage.h
        Isotope_table.cpp
        Isotope_table.h
)
