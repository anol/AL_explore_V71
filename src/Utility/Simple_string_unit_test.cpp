/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains
 * the property of Integrated Detector Electronics AS and its suppliers,
 * if any. The intellectual and technical concepts contained
 * herein are proprietary to Integrated Detector Electronics AS
 * and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Integrated Detector Electronics AS.
 */
/**
 * \date   IDEAS/28.10.2020/aeols
 * \brief
 */

#include "gtest/gtest.h"
import Utility.Simple_string;

using namespace Simple_string;

TEST(Simple_string_unit_test, test_compare_match) {
    const char match_rule[] = "abcd";
    const char search_string[] = "abcdX";
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 1));
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 2));
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 3));
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 4));
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, strlen(match_rule)));
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, strlen(search_string)));
}

TEST(Simple_string_unit_test, test_compare_mismatch) {
    const char match_rule[] = "abcd";
    const char search_string[] = "abcXX";
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 3));
    EXPECT_FALSE(wildcard_compare(match_rule, search_string, 4));
    EXPECT_FALSE(wildcard_compare(match_rule, search_string, 5));
}

TEST(Simple_string_unit_test, test_compare_wildcard) {
    const char match_rule[] = "abc\xFF";
    const char search_string[] = "abcXX";
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 3));
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 4));
    EXPECT_TRUE(wildcard_compare(match_rule, search_string, 5));
}

TEST(Simple_string_unit_test, test_compare_long_string) {
    const char match_A[10] = "abc\0";
    const char search_A[10] = "abcXX\0";
    EXPECT_TRUE(wildcard_compare(match_A, search_A, 10));
    const char match_B[10] = "abc\xFF\0";
    const char search_B[10] = "abcXX\0";
    EXPECT_TRUE(wildcard_compare(match_B, search_B, 10));
    const char match_C[10] = "abcd\0";
    const char search_C[10] = "abc\0";
    EXPECT_FALSE(wildcard_compare(match_C, search_C, 10));
}

TEST(Simple_string_unit_test, test_stringmask_to_bitmask_success) {
    int32_t mask = 0;
    EXPECT_TRUE (Simple_string::stringmask_to_bitmask("x", mask));
    EXPECT_EQ(mask, 0);
    EXPECT_TRUE (Simple_string::stringmask_to_bitmask("X", mask));
    EXPECT_EQ(mask, 0);
    EXPECT_TRUE (Simple_string::stringmask_to_bitmask("1", mask));
    EXPECT_EQ(mask, 1);
    EXPECT_TRUE (Simple_string::stringmask_to_bitmask("0", mask));
    EXPECT_EQ(mask, 3);
    EXPECT_TRUE (Simple_string::stringmask_to_bitmask("xxxxxx011", mask));
    EXPECT_EQ(mask, 0b110101);
    EXPECT_TRUE (Simple_string::stringmask_to_bitmask("01X00xxxxx011", mask));
    EXPECT_EQ(mask, 0b11010011110000000000110101);
}

TEST(Simple_string_unit_test, test_stringmask_to_bitmask_failure) {
    int32_t mask = 0;
    EXPECT_FALSE (Simple_string::stringmask_to_bitmask("z", mask));
    EXPECT_EQ(mask, 0);
    EXPECT_FALSE (Simple_string::stringmask_to_bitmask("", mask));
    EXPECT_EQ(mask, 0);
}
