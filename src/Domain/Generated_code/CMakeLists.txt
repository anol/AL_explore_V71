target_sources(${THE_TARGET_NAME} PUBLIC
        Domain_keyword_lookup.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET domain_generated_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Domain_keyword_lookup.cppm
        Domain_keyword_version.cppm
)
