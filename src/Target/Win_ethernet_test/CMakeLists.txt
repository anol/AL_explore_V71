target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
        mock_include
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Win_ethernet_test.cpp
)

# C o m p i l e r   a n d   l i n k e r   f l a g s

set(NPCAP_SDK_DIR "${SOURCE_ROOT_DIR}/../../npcap-sdk-1.16")

target_include_directories(${THE_TARGET_NAME} PUBLIC
        ${NPCAP_SDK_DIR}/include
)

target_compile_options(${THE_TARGET_NAME} PUBLIC
        -O0
        -g3
        -Wall
        -Wextra
        -fmessage-length=0
        -Wcast-qual
        -static-libgcc
        -static-libstdc++
)

add_compile_definitions(_WIN32_WINNT=0x0601)

target_link_options(${THE_TARGET_NAME} PUBLIC
        -Xlinker -Map=${PROJECT_NAME}.map
        -static
        -static-libgcc
        -static-libstdc++
)

find_library(WPCAP_LIB wpcap PATHS "${NPCAP_SDK_DIR}/Lib" REQUIRED)

find_library(PACKET_LIB Packet PATHS "${NPCAP_SDK_DIR}/Lib" REQUIRED)

target_link_libraries(${THE_TARGET_NAME} PRIVATE
        ${WPCAP_LIB}
        ${PACKET_LIB}
        #   wsock32
        ws2_32
        iphlpapi
)
