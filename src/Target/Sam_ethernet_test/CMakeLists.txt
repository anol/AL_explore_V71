
set(SAMV71_DIR "/Users/aeols/CLionProjects/Explore_SAMV71/src/Device/MCU/SAMV71")

target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
        ${SAMV71_DIR}
        ${SAMV71_DIR}/include
)

target_sources(${THE_TARGET_NAME} PUBLIC
        Sam_ethernet_test.cpp
)
