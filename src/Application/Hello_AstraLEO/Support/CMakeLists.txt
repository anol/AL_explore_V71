target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Cadence_control.cpp
        gamma_peak_detector.c
        gamma_peak_detector.h
        Isotope_table.cpp
        Isotope_table.h
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET application_support_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Cadence_control.cppm
)
