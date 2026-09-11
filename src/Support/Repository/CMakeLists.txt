target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Abstract_configuration.h
        Attribute_type.h
        Configuration_repository.cpp
        Configuration_repository.h
        Current_configuration.cpp
        Current_configuration.h
        Embedded_flash.h
        Persistent_storage.h
        Repository_diagnostics.h
)
