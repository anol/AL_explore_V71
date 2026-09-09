target_include_directories(${THE_TARGET_NAME} PUBLIC
        .
)
target_sources(${THE_TARGET_NAME} PUBLIC
#        Chunk_queue_unit_test.cpp
#        List_unit_test.cpp
        Simple_math.h
        Histogram.h
#        Size_adapter_unit_test.cpp
        Json_printer.h
#        Simple_string_unit_test.cpp
        Simple_bitset.h
        Chunk_queue.h
        Ringbuffer.h
        Simple_string.cpp
        Simple_math.cpp
        Unit_converter.h
#        Histogram_unit_test.cpp
#        Simple_buffer_unit_test.cpp
        IChunk_queue.h
        CRC_16_CCITT.h
#        Unit_converter_unit_test.cpp
        Size_adapter.cpp
        Simple_string.h
        Utility_types.h
        Gray_code.h
#        Simple_bitset_unit_test.cpp
        CRC_16_CCITT.cpp
#        CRC_16_CCITT_unit_test.cpp
#        Ringbuffer_unit_test.cpp
#        Gray_code_unit_test.cpp
        Simple_buffer.h
        Json_printer.cpp
        Size_adapter.h
#        Simple_math_unit_test.cpp
)
