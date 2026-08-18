//
// Created by anolsen on 11.05.2020.
//

#include "gtest/gtest.h"
#include "Unit_converter.h"

TEST(Unit_converter_unit_test, test_nanoseconds_to_count) {
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(1000000, 1000), 1);
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(16000000, 20000000), 320000);
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(50000000, 1000000), 50000);
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(1000000000, 1000000000), 1000000000);
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(30'100'000, 498), 15);
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(30'100'000, 1007), 30);
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(30'010'000, 2007), 60);
    EXPECT_EQ(Unit_converter::nanoseconds_to_count(29'999'999, 3007), 90);
}

TEST(Unit_converter_unit_test, test_nanoseconds_to_uint16_count) {
    uint32_t pulse_width_ns = 2'000'000;
    uint32_t clock_frequency = 30'000'000;
    uint32_t puls_count = (pulse_width_ns / 1000) * (clock_frequency / 1'000'000);
    EXPECT_EQ(puls_count, (uint16_t) (0xFFFF & puls_count));
}

TEST(Unit_converter_unit_test, test_nanoseconds_uint16_vs_double) {
    uint32_t pulse_width_ns = 2'000'000;
    uint32_t clock_frequency = 30'000'000;
    uint32_t puls_count = (pulse_width_ns / 1000) * (clock_frequency / 1'000'000);
    EXPECT_EQ(puls_count, Unit_converter::nanoseconds_to_count(clock_frequency, pulse_width_ns));
}

TEST(Unit_converter_unit_test, test_nanoseconds_overrun_uint16) {
    uint32_t pulse_width_ns = 3'000'000;
    uint32_t clock_frequency = 30'000'000;
    uint32_t puls_count = (pulse_width_ns / 1000) * (clock_frequency / 1'000'000);
    EXPECT_NE(puls_count, (uint16_t) (0xFFFF & puls_count));
}