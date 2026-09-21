//
// Created by anolsen on 11.05.2020.
//


#include "gtest/gtest.h"
import Utility.Simple_math;

TEST(Simple_math_unit_test, test_square_root) {
    EXPECT_EQ(Simple_math::square_root(1.0), 1.0);
    EXPECT_EQ(Simple_math::square_root(4.0), 2.0);
    EXPECT_EQ(Simple_math::square_root(25.0), 5.0);
    EXPECT_EQ(Simple_math::square_root(64.0), 8.0);
}

TEST(Simple_math_unit_test, test_standard_deviation_1) {
    EXPECT_EQ(Simple_math::standard_deviation(2 + 2 + 2, 4 + 4 + 4, 3), 0.0);
    EXPECT_EQ(Simple_math::standard_deviation(1 + 2 + 3, 1 + 4 + 9, 3), 1.0);
    EXPECT_NEAR(Simple_math::standard_deviation(16.0 + 25.0 + 36.0, 256.0 + 625.0 + 1296.0, 3.0), 10.0, 0.1);
}

TEST(Simple_math_unit_test, test_standard_deviation_2) {
    double samples[] = {791, 784, 793, 779, 794, 754, 803, 811, 798, 785, 756, 771, 783, 787, 763, 775, 780, 780, 773,
                        751, 789, 767, 778, 749, 785, 801, 758, 765, 797, 782, 765, 785, 787, 791, 782, 795, 769, 761,
                        792, 770, 788, 762, 774, 765, 771, 804, 779, 805, 781, 799, 794, 747, 775, 779, 779, 771, 739,
                        775, 788, 788, 779, 793, 763, 800, 759, 783, 803, 792, 773, 795, 781, 789, 779, 778, 764, 783,
                        739, 771, 789, 775, 769, 795, 774, 799, 789, 759, 763, 802, 777, 795, 787, 777, 783, 788, 772,
                        821, 793, 783, 772, 803, 761, 789, 799, 776, 782, 767, 771, 799, 769, 797, 778, 830, 778, 753,
                        778, 765, 785, 778, 781, 769, 780, 772, 774, 781, 770, 807, 776, 751, 767, 778, 779, 775, 795,
                        751, 782, 781, 778, 776, 788, 801, 782, 780, 776, 773, 770, 775, 786, 780, 777, 773, 789, 784,
                        774, 745, 770, 779, 748, 765, 755, 772, 770, 796, 775, 773, 766, 791, 791, 783, 794, 802, 779,
                        780, 765, 790, 764, 764, 784, 772, 768, 790, 771, 779, 799, 803, 791, 787, 772, 752, 775, 803,
                        741, 787, 777, 759, 767, 782, 776, 759, 782, 785, 776};
    double number_of_samples = 0.0;
    double sum_of_samples = 0.0;
    double sum_of_squares = 0.0;
    for (double sample : samples) {
        number_of_samples += 1.0;
        sum_of_samples += sample;
        sum_of_squares += sample * sample;
    }
    EXPECT_NEAR(Simple_math::standard_deviation(sum_of_samples, sum_of_squares, number_of_samples), 15.0, 0.2);
}
