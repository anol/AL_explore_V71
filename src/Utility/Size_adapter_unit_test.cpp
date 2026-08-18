//
// Created by anolsen on 22.04.2020.
//

#include "gtest/gtest.h"
#include "Size_adapter.h"

TEST(Size_adapter_unit_test, test_store_and_fetch) {
    uint32_t source = 0x01234567;
    uint32_t destination = 0;
    uint32_t *p_source = &source;
    uint32_t *p_destination = &destination;
    Size_adapter::store_byte(p_destination, 0, Size_adapter::fetch_byte(p_source, 0));
    EXPECT_EQ(destination, 0x01000000);
    Size_adapter::store_byte(p_destination, 1, Size_adapter::fetch_byte(p_source, 1));
    EXPECT_EQ(destination, 0x01230000);
    Size_adapter::store_byte(p_destination, 2, Size_adapter::fetch_byte(p_source, 2));
    EXPECT_EQ(destination, 0x01234500);
    Size_adapter::store_byte(p_destination, 3, Size_adapter::fetch_byte(p_source, 3));
    EXPECT_EQ(destination, 0x01234567);
}

TEST(Size_adapter_unit_test, test_array) {
    enum {
        V0 = 0x01234567, V1 = 0x89ABCDEF, V2 = 0xFEDCBA98, V3 = 0x76543210
    };
    uint32_t source[4] = {V0, V1, V2, V3};
    uint32_t destination[4] = {0, 0, 0, 0};
    uint32_t *p_source = source;
    uint32_t *p_destination = destination;
    for (int count = 0; count < 16; count++) {
        uint8_t data = Size_adapter::fetch_byte(p_source, count);
        Size_adapter::store_byte(p_destination, count, data);
    }
    EXPECT_EQ(destination[0], V0);
    EXPECT_EQ(destination[1], V1);
    EXPECT_EQ(destination[2], V2);
    EXPECT_EQ(destination[3], V3);
}

TEST(Size_adapter_unit_test, test_swap) {
    EXPECT_EQ(Size_adapter::swap_shorts(0x0123'4567), 0x4567'0123);
    EXPECT_EQ(Size_adapter::swap_endian(0x0123'4567), 0x6745'2301);
}
