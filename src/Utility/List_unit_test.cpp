/*
 * Copyright (C) 2015 Integrated Detector Electronics AS
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
 * \file   List_unit_test.cpp
 * \author cplaissy, IDEAS
 * \date   2021-05-14
 * \brief  
 */

#include "gtest/gtest.h"
#include "List.h"

TEST(List_unit_test, test_empty)
{
    int data;
    int *ptr = &data;
    List<int, 20> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_FALSE(list.is_full());
    EXPECT_EQ(list.get(0), nullptr);
    EXPECT_EQ(list.get(1), nullptr);
    EXPECT_EQ(list.get(2), nullptr);
    EXPECT_EQ(list.get(3), nullptr);
    EXPECT_EQ(list.get(19), nullptr);
    EXPECT_EQ(list.get(20), nullptr); // out of range
    EXPECT_FALSE(list.pop(ptr));
    EXPECT_EQ(list.get_size(), 0);
    EXPECT_EQ(list.get_capacity(), 20);
    EXPECT_FALSE(list.for_each(nullptr, [](void* user, int& el, void* context) -> List<int, 20>::for_each_ret
    {
        return List<int, 20>::for_each_ret::for_each_break;
    }));
    EXPECT_EQ(list.get_max_fill(), 0);
    EXPECT_EQ(list.get_put_count(), 0);
}


TEST(List_unit_test, test_push)
{
    List<int, 20> list;
    EXPECT_TRUE(list.is_empty());
    for (uint32_t i = 0; i < 20; ++i)
    {
        EXPECT_TRUE(list.push(i + 5) != nullptr);
    }
    EXPECT_FALSE(list.push(20 + 5) != nullptr);
    EXPECT_TRUE(list.is_full());
    EXPECT_EQ(list.get_max_fill(), 20);
    EXPECT_EQ(list.get_put_count(), 20);
    EXPECT_EQ(list.get_overflow_count(), 1);
}


TEST(List_unit_test, test_get)
{
    int data = 30;
    List<int, 20> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_TRUE(list.push(data) != nullptr);
    int* ptr = list.get(0);
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_TRUE(*ptr == data);
    ptr = list.get(1);
    EXPECT_TRUE(ptr == nullptr);
}


TEST(List_unit_test, test_pop)
{
    int data = 30;
    List<int, 20> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_TRUE(list.push(data) != nullptr);
    int* ptr = list.get(0);
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_TRUE(*ptr == data);
    EXPECT_TRUE(list.pop(ptr));
    EXPECT_TRUE(list.is_empty());
    ptr = list.get(0);
    EXPECT_FALSE(ptr != nullptr);
}


TEST(List_unit_test, test_sort)
{
    int data[10] = {12, 13, 14, 15, 11, 10, 19, 17, 18, 16};
    List<int, 20> list;
    EXPECT_TRUE(list.is_empty());
    for (auto &d : data)
    {
        EXPECT_TRUE(list.push(d) != nullptr);
    }
    list.sort([](const int &lhs, const int &rhs) -> bool
    {
        return lhs < rhs;
    });
    int *ptr;
    for (uint32_t i = 0; i < 10; ++i)
    {
        ptr = list.get(i);
        EXPECT_TRUE(*ptr == (i + 10));
    }
}


TEST(List_unit_test, test_for_each)
{
    int data[10] = {12, 13, 14, 15, 11, 10, 19, 17, 18, 16};
    List<int, 20> list;
    EXPECT_TRUE(list.is_empty());
    for (auto &d : data)
    {
        EXPECT_TRUE(list.push(d) != nullptr);
    }
    int expected = 10;
    struct Expected
    {
        uint32_t index;
        int *data;
    };
    Expected context{0, data};
    EXPECT_TRUE(list.for_each(nullptr, [](void* user, int& el, void* context) -> List<int, 20>::for_each_ret
    {
        auto expected = (Expected*)context;
        EXPECT_EQ(expected->data[expected->index], el);
        expected->index++;
        return List<int, 20>::for_each_ret::for_each_continue;
    }, &context));
}


