target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Misc_type.h
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET type_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Status_code.cppm
)
add_subdirectory(Abstract)
add_subdirectory(Generic)


