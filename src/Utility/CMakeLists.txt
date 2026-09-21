target_sources(${THE_TARGET_NAME} PUBLIC
#        Chunk_queue_unit_test.cpp
#        List_unit_test.cpp
#        Size_adapter_unit_test.cpp
#        Simple_string_unit_test.cpp
#        Histogram_unit_test.cpp
#        Simple_buffer_unit_test.cpp
#        Unit_converter_unit_test.cpp
#        Simple_bitset_unit_test.cpp
#        CRC_16_CCITT_unit_test.cpp
#        Ringbuffer_unit_test.cpp
#        Gray_code_unit_test.cpp
#        Simple_math_unit_test.cpp
        CRC_16_CCITT.cpp
        Json_printer.cpp
        Simple_math.cpp
        Simple_string.cpp
        Size_adapter.cpp
)
target_sources(${THE_TARGET_NAME} PRIVATE
        FILE_SET utility_modules TYPE CXX_MODULES BASE_DIRS . FILES
        Chunk_queue.cppm
        CRC_16_CCITT.cppm
        Gray_code.cppm
        Histogram.cppm
        IChunk_queue.cppm
        Json_printer.cppm
        Ringbuffer.cppm
        Simple_bitset.cppm
        Simple_buffer.cppm
        Simple_math.cppm
        Simple_string.cppm
        Size_adapter.cppm
        Unit_converter.cppm
)
