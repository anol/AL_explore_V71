target_include_directories(${THE_TARGET_NAME} PUBLIC
        ../../Unit_test
        mock_include
)
target_sources(${THE_TARGET_NAME} PUBLIC
        Unit_test.cpp
        mock_include/iphlpapi.h
        mock_include/pcap.h
        mock_include/winsock2.h
)
