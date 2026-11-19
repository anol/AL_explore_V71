target_sources(${THE_TARGET_NAME} PUBLIC
        Temperature_task.cpp
#        Sensor_STTS22H.cpp
#        Sensor_STTS22H.h
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET component_temperature_sensor TYPE CXX_MODULES BASE_DIRS . FILES
        Temperature_task.cppm
)