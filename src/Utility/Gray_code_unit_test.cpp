//
// Created by anolsen on 03.06.2020.
//

#include "gtest/gtest.h"
import Utility.Gray_code;

TEST(Gray_code_unit_test, test_gray_to_normal) {
    EXPECT_EQ(Gray_code::gray_to_normal(0), 0);
    EXPECT_EQ(Gray_code::gray_to_normal(1), 1);
    EXPECT_EQ(Gray_code::gray_to_normal(3), 2);
    EXPECT_EQ(Gray_code::gray_to_normal(2), 3);
    EXPECT_EQ(Gray_code::gray_to_normal(6), 4);
    EXPECT_EQ(Gray_code::gray_to_normal(7), 5);
    EXPECT_EQ(Gray_code::gray_to_normal(5), 6);
    EXPECT_EQ(Gray_code::gray_to_normal(4), 7);
    EXPECT_EQ(Gray_code::gray_to_normal(12), 8);
    EXPECT_EQ(Gray_code::gray_to_normal(13), 9);
    EXPECT_EQ(Gray_code::gray_to_normal(15), 10);
    EXPECT_EQ(Gray_code::gray_to_normal(14), 11);
    EXPECT_EQ(Gray_code::gray_to_normal(10), 12);
    EXPECT_EQ(Gray_code::gray_to_normal(11), 13);
    EXPECT_EQ(Gray_code::gray_to_normal(9), 14);
    EXPECT_EQ(Gray_code::gray_to_normal(8), 15);
    EXPECT_EQ(Gray_code::gray_to_normal(16384), 32767);
    EXPECT_EQ(Gray_code::gray_to_normal(0b100000000000000000000), 0b111111111111111111111);
}
