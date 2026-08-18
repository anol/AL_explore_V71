//
// Created by Drift on 02.09.2020.
//


#include "gtest/gtest.h"
#include "Simple_bitset.h"

TEST(Simple_bitset_unit_test, test_set_bit) {
    Simple_bitset<68> bitset;
    bitset.set_bit(23, true);
    EXPECT_TRUE(bitset.test_bit(23));
    bitset.set_bit(23, false);
    EXPECT_FALSE(bitset.test_bit(23));
    bitset.raise_bit(23);
    EXPECT_TRUE(bitset.test_bit(23));
    bitset.clear_bit(23);
    EXPECT_FALSE(bitset.test_bit(23));
}

TEST(Simple_bitset_unit_test, test_test_bit) {
    Simple_bitset<68> bitset;
    for (int i = 0; i < 68; ++i) {
        bitset.raise_bit(i);
    }
    for (int i = 0; i < 68; ++i) {
        EXPECT_TRUE(bitset.test_bit(i));
    }
    for (int i = 0; i < 68; ++i) {
        bitset.clear_bit(i);
    }
    for (int i = 0; i < 68; ++i) {
        EXPECT_FALSE(bitset.test_bit(i));
    }
}

