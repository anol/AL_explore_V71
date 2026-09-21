//
// Created by anolsen on 03.06.2020.
//

#include "gtest/gtest.h"
import Utility.Ringbuffer;

TEST(Ringbuffer_unit_test, test_empty) {
    int data;
    Ringbuffer<int, 20> buffer;
    EXPECT_TRUE(buffer.is_empty());
    EXPECT_FALSE(buffer.peek(&data));
    EXPECT_FALSE(buffer.get(&data));
    EXPECT_FALSE(buffer.is_getting_filled());
    EXPECT_EQ(buffer.get_fill(), 0);
    EXPECT_EQ(buffer.get_size(), 20);
    EXPECT_FALSE(buffer.for_each(nullptr, nullptr));
}

TEST(Ringbuffer_unit_test, test_get) {
    int data;
    Ringbuffer<int, 20> buffer;
    for (int gpc = 0; gpc < 5; gpc++) {
        EXPECT_TRUE(buffer.put(gpc));
    }
    EXPECT_FALSE(buffer.is_empty());
    EXPECT_EQ(buffer.get_fill(), 5);
    EXPECT_TRUE(buffer.peek(&data));
    EXPECT_EQ(data, 0);
    EXPECT_EQ(buffer.get_fill(), 5);
    EXPECT_TRUE(buffer.get(&data));
    EXPECT_EQ(data, 0);
    EXPECT_EQ(buffer.get_fill(), 4);
    EXPECT_TRUE(buffer.get(&data));
    EXPECT_EQ(data, 1);
    EXPECT_EQ(buffer.get_fill(), 3);
    EXPECT_TRUE(buffer.for_each(nullptr, [](Optional_user, User_data) { return true; }));
}

TEST(Ringbuffer_unit_test, test_put) {
    int data = 1;
    Ringbuffer<int, 20> buffer;
    for (int gpc = 0; gpc < 18; gpc++) {
        EXPECT_TRUE(buffer.put(gpc));
    }
    EXPECT_TRUE(buffer.is_getting_filled());
    EXPECT_FALSE(buffer.put(data));
}

TEST(Ringbuffer_unit_test, test_simple_chunks) {
    enum {
        Buffer_size = 22
    };
    Ringbuffer<uint32_t, Buffer_size> buffer;
    uint32_t put_data[]{0, 1, 2, 3, 4, 5, 6};
    uint32_t put_size = sizeof(put_data) / sizeof(uint32_t);
    put_data[0] = put_size;
    // 1st put
    ASSERT_TRUE(buffer.put(put_data, put_size));
    ASSERT_EQ(buffer.get_free(), Buffer_size - put_size);
    // 2nd put
    ASSERT_TRUE(buffer.put(put_data, put_size));
    ASSERT_EQ(buffer.get_free(), Buffer_size - (2 * put_size));
    // 3rd put
    ASSERT_TRUE(buffer.put(put_data, put_size));
    ASSERT_EQ(buffer.get_free(), Buffer_size - (3 * put_size));
    // The 4th put fails since there are no room
    ASSERT_FALSE(buffer.put(put_data, put_size));
    uint32_t get_data[10]{};
    // 1st get
    ASSERT_TRUE(buffer.get_exact(get_data, 1));
    ASSERT_EQ(*get_data, put_size);
    ASSERT_TRUE(buffer.get_exact(get_data + 1, (*get_data) - 1));
    // 2nd get
    ASSERT_TRUE(buffer.get_exact(get_data, 1));
    ASSERT_EQ(*get_data, put_size);
    ASSERT_TRUE(buffer.get_exact(get_data + 1, (*get_data) - 1));
    // 3rd get
    ASSERT_TRUE(buffer.get_exact(get_data, 1));
    ASSERT_EQ(*get_data, put_size);
    ASSERT_TRUE(buffer.get_exact(get_data + 1, (*get_data) - 1));
    // The 4th get fails since there are noe more data
    ASSERT_FALSE(buffer.get_exact(get_data, 1));
}

TEST(Ringbuffer_unit_test, test_varying_chunks) {
    enum {
        Queue_size = 75
    };
    Ringbuffer<uint32_t, Queue_size> queue;
    uint32_t put_data[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uint32_t put_size = sizeof(put_data) / sizeof(int);
    uint32_t counter{};
    for (counter = 0; counter < put_size; counter++) {
        put_data[0] = counter;
        if (!queue.put(put_data, counter + 1)) {
            break;
        }
    }
    EXPECT_EQ(counter, 11);
    ASSERT_EQ(queue.get_fill(), 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11);
    uint32_t get_data[20]{};
    uint32_t get_size{};
    ASSERT_TRUE(queue.get_exact(&get_size, 1));
    ASSERT_EQ(get_size, 0);
    ASSERT_TRUE(queue.get_exact(&get_size, 1));
    ASSERT_EQ(get_size, 1);
    ASSERT_TRUE(queue.get_exact(get_data, get_size));
    ASSERT_TRUE(queue.get_exact(&get_size, 1));
    ASSERT_EQ(get_size, 2);
    ASSERT_TRUE(queue.get_exact(get_data, get_size));
    ASSERT_TRUE(queue.get_exact(&get_size, 1));
    ASSERT_EQ(get_size, 3);
    ASSERT_TRUE(queue.get_exact(get_data, get_size));
    ASSERT_TRUE(queue.get_exact(&get_size, 1));
    ASSERT_EQ(get_size, 4);
    ASSERT_TRUE(queue.get_exact(get_data, get_size));
    ASSERT_TRUE(queue.get_exact(&get_size, 1));
    ASSERT_EQ(get_size, 5);
    ASSERT_TRUE(queue.get_exact(get_data, get_size));
    ASSERT_TRUE(queue.get_exact(&get_size, 1));
    ASSERT_EQ(get_size, 6);
    ASSERT_TRUE(queue.get_exact(get_data, get_size));
    uint32_t write{};
    for (write = counter; write < put_size; write++) {
        put_data[0] = write;
        if (!queue.put(put_data, write + 1)) {
            break;
        }
    }
    EXPECT_EQ(write, 13);
    EXPECT_TRUE(queue.get_exact(&get_size, 1));
    EXPECT_EQ(get_size, 7);
    EXPECT_TRUE(queue.get_exact(get_data, get_size));
    EXPECT_TRUE(queue.get_exact(&get_size, 1));
    EXPECT_EQ(get_size, 8);
    EXPECT_TRUE(queue.get_exact(get_data, get_size));
    EXPECT_TRUE(queue.get_exact(&get_size, 1));
    EXPECT_EQ(get_size, 9);
    EXPECT_TRUE(queue.get_exact(get_data, get_size));
    EXPECT_TRUE(queue.get_exact(&get_size, 1));
    EXPECT_EQ(get_size, 10);
    EXPECT_TRUE(queue.get_exact(get_data, get_size));
    EXPECT_TRUE(queue.get_exact(&get_size, 1));
    EXPECT_EQ(get_size, 11);
    EXPECT_TRUE(queue.get_exact(get_data, get_size));
    EXPECT_TRUE(queue.get_exact(&get_size, 1));
    EXPECT_EQ(get_size, 12);
    EXPECT_TRUE(queue.get_exact(get_data, get_size));
    ASSERT_EQ(queue.get_fill(), 0);

}


TEST(Ringbuffer_unit_test, test_put_front)
{
    /// @todo write extensive Ringbuffer::put_front() unit tests
    enum {
        Queue_size = 75
    };
    Ringbuffer<uint32_t, Queue_size> queue;
    uint32_t d32{0xABBA'BABE};

    queue.put_front(d32);
}


TEST(Ringbuffer_unit_test, test_peek)
{
    enum {
        Queue_size = 10
    };
    Ringbuffer<uint8_t, Queue_size> queue;
    uint8_t d32[4U]{0xAB, 0xBA, 0xBA, 0xBE};
    uint8_t readback[Queue_size]{};

    queue.put(d32, 4U);

    EXPECT_EQ(queue.get_fill(), 4U);
    EXPECT_EQ(queue.peek(readback, Queue_size), 4U);
    for (size_t i = 0; i < 4; ++i)
    {
        EXPECT_EQ(readback[i], d32[i]);
    }
}