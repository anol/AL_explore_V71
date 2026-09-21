target_sources(${THE_TARGET_NAME} PUBLIC
        Configuration_repository.cpp
        Current_configuration.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET support_repository_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Abstract_configuration.cppm
        Attribute_type.cppm
        Configuration_repository.cppm
        Current_configuration.cppm
        Embedded_flash.cppm
        Mockup_persistent_storage.cppm
        Persistent_storage.cppm
        Repository_diagnostics.cppm
)
