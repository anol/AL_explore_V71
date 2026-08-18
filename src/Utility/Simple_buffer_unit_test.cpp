/**
 * @file   SimpleBuffer_unit_test.cpp
 * @author ChristophePlaissy, IDEAS
 * @date   04/11/2022
 * @brief  
 */

#include "gtest/gtest.h"
#include "Simple_buffer.h"

TEST(SimpleBuffer, generic_test) // NOLINT suppress static storage warning
{
    { // int SimpleBuffer scope, test some other types
        using buffer_t = Simple_buffer<int, 10>;
        buffer_t buffer;


        EXPECT_TRUE(buffer.empty());
        EXPECT_FALSE(buffer.full());
        EXPECT_EQ(buffer.size(), 0);
        EXPECT_EQ(buffer.available(), buffer.capacity());

        for (int i = 1; i < buffer.capacity() + 1; ++i)
        {
            if (buffer.push_back(i))
            {
                EXPECT_FALSE(buffer.empty());
                EXPECT_EQ(buffer.size(), i);
                EXPECT_EQ(buffer[i - 1], i);
                EXPECT_EQ(buffer.at(i - 1), i);
                EXPECT_EQ(buffer.front(), 1);
                EXPECT_EQ(buffer.back(), i);
                EXPECT_EQ(buffer.available(), buffer.capacity() - i);
            }
            else break;
        }

        EXPECT_TRUE(buffer.full());

        int content{1};
        for (auto &element : buffer) // auto iterability
        {
            EXPECT_EQ(element, content);
            content++;
        }

        content = 1;
        for (const auto &element : buffer) // const auto iterability
        {
            EXPECT_EQ(element, content);
            content++;
        }

        content = 1;
        for (buffer_t::iterator it = buffer.begin(); it != buffer.end(); ++it) // NOLINT iterability
        {
            EXPECT_EQ(*it, content);
            content++;
        }

        content = 1;
        for (buffer_t::const_iterator it = buffer.cbegin(); it != buffer.cend(); ++it) // NOLINT const iterability
        {
            EXPECT_EQ(*it, content);
            content++;
        }

        content = buffer.back();
        for (buffer_t::reverse_iterator it = buffer.rbegin(); it != buffer.rend(); ++it) // reverse iterability
        {
            EXPECT_EQ(*it, content);
            content--;
        }

        content = buffer.back();
        for (buffer_t::const_reverse_iterator it = buffer.crbegin(); it != buffer.crend(); ++it) // const reverse iterability
        {
            EXPECT_EQ(*it, content);
            content--;
        }
    }



    {
        struct some_struct
        {
            some_struct() = default;
            some_struct(const some_struct &other)
            {
                ptr = other.ptr;
                uint = other.uint;
                flg = other.flg;
            }
            some_struct &operator=(const some_struct &rhs)
            {
                if (this != &rhs)
                {
                    ptr = rhs.ptr;
                    uint = rhs.uint;
                    flg = rhs.flg;
                }
                return *this;
            }
            some_struct(void *addr, uint32_t size, bool active = true)
            : ptr(addr), uint(size), flg(active) {}

            void *ptr{};
            uint32_t uint{};
            bool flg{};
        };

        int data;
        using buffer_t = Simple_buffer<some_struct, 10>;
        buffer_t buffer;
        buffer.push_back({&data, 1, true});
        buffer.push_back({&data, 20, false});

        EXPECT_EQ(buffer.front().ptr, &data);
        EXPECT_EQ(buffer.front().uint, 1);
        EXPECT_EQ(buffer.front().flg, true);
        EXPECT_EQ(buffer.back().ptr, &data);
        EXPECT_EQ(buffer.back().uint, 20);
        EXPECT_EQ(buffer.back().flg, false);
    }
}