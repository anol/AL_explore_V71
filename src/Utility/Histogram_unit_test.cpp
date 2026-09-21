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
 * \date   IDEAS/22.06.2021/aeols
 * \brief Histogram unit test
 */

#include "gtest/gtest.h"
#include "Utility_types.h"
import Utility.Histogram;

TEST(Histogram_unit_test, test_one_to_one) {
    Histogram<4> histogram;
    EXPECT_TRUE(histogram.initialize(1, 4, 1));
    histogram.count_hit(1);
    histogram.count_hit(2);
    histogram.count_hit(3);
    histogram.count_hit(4);
    EXPECT_EQ(histogram.get_hits(0), 1);
    EXPECT_EQ(histogram.get_hits(1), 1);
    EXPECT_EQ(histogram.get_hits(2), 1);
    EXPECT_EQ(histogram.get_hits(3), 1);
}

TEST(Histogram_unit_test, test_wrong_parameters) {
    Histogram<4> histogram;
    EXPECT_TRUE(histogram.initialize(0, 4, 1));
    EXPECT_FALSE(histogram.initialize(1, 5, 1));
    EXPECT_FALSE(histogram.initialize(1, 4, 0));
    EXPECT_FALSE(histogram.initialize(1, 0, 1));
}

TEST(Histogram_unit_test, test_bin_span) {
    Histogram<4> histogram;
    EXPECT_TRUE(histogram.initialize(1, 4, 1));
    EXPECT_EQ(histogram.get_bin_span(), 1);
    EXPECT_TRUE(histogram.initialize(1, 4, 2));
    EXPECT_EQ(histogram.get_bin_span(), 2);
    EXPECT_TRUE(histogram.initialize(1, 4, 3));
    EXPECT_EQ(histogram.get_bin_span(), 3);
}

TEST(Histogram_unit_test, test_out_of_range) {
    Histogram<5> histogram;
    EXPECT_TRUE(histogram.initialize(12, 5, 15));
    histogram.count_hit(1);
    histogram.count_hit(2);
    histogram.count_hit(27);
    histogram.count_hit(28);
    histogram.count_hit(42);
    histogram.count_hit(1000);
    EXPECT_EQ(histogram.get_hits(0), 2);
    EXPECT_EQ(histogram.get_hits(1), 2);
    EXPECT_EQ(histogram.get_hits(2), 1);
    EXPECT_EQ(histogram.get_hits(3), 0);
    EXPECT_EQ(histogram.get_hits(4), 1);
}
