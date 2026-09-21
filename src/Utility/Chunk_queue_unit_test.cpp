//
// Created by AndersEmilOlsen on 01.12.2023.
//

#include "gtest/gtest.h"

import Utility.Chunk_queue;


TEST(Chunk_queue_unit_test, test_empty) {
    enum : uint32_t {
        Data_size = 10,
        Queue_size = 20,
    };
    Utility::Chunk_queue<Queue_size> queue;
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.chunk_count(), 0);
    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(queue.available(), Queue_size);
    EXPECT_EQ(queue.capacity(), Queue_size);
    uint8_t data[Data_size]{};
    EXPECT_EQ(0, queue.pop_front(data, Data_size));
}

TEST(Chunk_queue_unit_test, test_put) {
    enum : uint32_t {
        Data_size = 7,
        Queue_size = 33,
    };
    Utility::Chunk_queue<Queue_size> queue;
    uint8_t data[Data_size]{};
    EXPECT_TRUE(queue.push_back(data, Data_size));
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.chunk_count(), 1);
    EXPECT_EQ(queue.size(), Data_size + 4);
    EXPECT_EQ(queue.available(), Queue_size - Data_size - 4);
}

TEST(Chunk_queue_unit_test, test_size) {
    enum : uint32_t {
        Data_size = 7,
        Queue_size = 33,
    };
    Utility::Chunk_queue<Queue_size> queue;
    uint8_t data[Data_size]{};
    EXPECT_TRUE(queue.push_back(data, Data_size));
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.chunk_count(), 1);
    EXPECT_EQ(queue.size(), Data_size + 4);
    EXPECT_EQ(queue.available(), Queue_size - Data_size - 4);
    EXPECT_EQ(queue.front_size(), Data_size);
    EXPECT_EQ(queue.front_size(), Data_size);
    EXPECT_EQ(queue.front_size(), Data_size);
    EXPECT_EQ(queue.chunk_count(), 1);
}



TEST(Chunk_queue_unit_test, test_get) {
    enum : uint32_t {
        Data_size = 11,
        Buffer_size = Data_size + 1,
        Queue_size = 17,
    };
    Utility::Chunk_queue<Queue_size> queue;
    uint8_t put_data[Data_size]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    ASSERT_TRUE(queue.push_back(put_data, Data_size));

    uint8_t buffer[Buffer_size];
    EXPECT_EQ(Data_size, queue.pop_front(buffer, Buffer_size));

    EXPECT_TRUE(0 == memcmp(put_data, buffer, sizeof(put_data)));
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.chunk_count(), 0);
    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(queue.available(), Queue_size);
    EXPECT_EQ(queue.capacity(), Queue_size);
    EXPECT_EQ(0, queue.pop_front(buffer, Buffer_size));
}

TEST(Chunk_queue_unit_test, test_wraping) {
    enum : uint32_t {
        Buffer_size = 50,
        Queue_size = 80,
    };
    Utility::Chunk_queue<Queue_size> queue;
    uint8_t aaaa[]
            {1, 2, 8, 8, 5, 4, 3, 2, 6, 1, 6, 8, 4, 3, 4, 5, 6, 7, 8, 19, 0, 43, 2, 1, 2, 54, 56, 67, 78, 8, 34, 22};
    uint32_t aaaa_size = sizeof(aaaa);
    ASSERT_GT(aaaa_size, 30);
    uint8_t bbbb[]
            {8, 255, 3, 6, 9, 255, 3, 2, 5, 6, 7, 8, 9, 2, 3, 4, 5, 6, 7, 7, 3, 2, 2, 1, 7, 78, 4, 8, 3, 1, 9, 5, 4};
    uint32_t bbbb_size = sizeof(aaaa);
    ASSERT_GT(bbbb_size, 30);
    uint8_t cccc[]
            {9, 4, 3, 6, 7, 2, 33, 4, 88, 5, 56, 3, 4, 9, 7, 1, 2, 3, 4, 255, 7, 3, 6, 9, 16, 5, 3, 34, 255, 42, 54};
    uint32_t cccc_size = sizeof(aaaa);
    ASSERT_GT(cccc_size, 30);

    uint8_t data[Buffer_size]{};
    for (auto redo = 0; redo < 100; redo++) {
        ASSERT_TRUE(queue.push_back(aaaa, aaaa_size));
        ASSERT_TRUE(queue.push_back(bbbb, bbbb_size));
        ASSERT_EQ(aaaa_size, queue.pop_front(data, Buffer_size));
        ASSERT_TRUE(0 == memcmp(aaaa, data, aaaa_size));
        ASSERT_TRUE(queue.push_back(cccc, cccc_size));
        ASSERT_EQ(bbbb_size, queue.pop_front(data, Buffer_size));
        ASSERT_TRUE(0 == memcmp(bbbb, data, bbbb_size));
        ASSERT_EQ(cccc_size, queue.pop_front(data, Buffer_size));
        ASSERT_TRUE(0 == memcmp(cccc, data, cccc_size));
    }
}

TEST(Chunk_queue_unit_test, test_overflow_1) {
    enum : uint32_t {
        Buffer_size = 40,
        Queue_size = 33,
    };
    Utility::Chunk_queue<Queue_size> queue;
    uint8_t data[Buffer_size]{};
    EXPECT_FALSE(queue.push_back(data, Queue_size + 1));
    EXPECT_FALSE(queue.push_back(data, Queue_size));
    EXPECT_FALSE(queue.push_back(data, Queue_size - 1));
    EXPECT_TRUE(queue.push_back(data, Queue_size - 5));
}

TEST(Chunk_queue_unit_test, test_overflow_2) {
    enum : uint32_t {
        Buffer_size = 40,
        Queue_size = 40,
        Put_size = 20,
    };
    Utility::Chunk_queue<Queue_size> queue;
    uint8_t data[Buffer_size]{};
    EXPECT_TRUE(queue.push_back(data, Put_size));
    EXPECT_FALSE(queue.push_back(data, Put_size + 1));
    EXPECT_FALSE(queue.push_back(data, Put_size));
    EXPECT_FALSE(queue.push_back(data, Put_size - 1));
    EXPECT_FALSE(queue.push_back(data, Put_size - 2));
    EXPECT_TRUE(queue.push_back(data, Put_size - 15));
}


TEST(Chunk_queue_unit_test, test_peek) {
    enum : uint32_t {
        Buffer_size = 40,
        Queue_size = 40,
        Put_size = 20,
    };
    Utility::Chunk_queue<Queue_size> queue;
    uint8_t data[Buffer_size]{};
    uint8_t readback[Buffer_size]{};
    for (size_t i = 0; i < Buffer_size; ++i) data[i] = i;

    EXPECT_TRUE(queue.push_back(data, Put_size));
    EXPECT_EQ(queue.front_size(), Put_size);
    EXPECT_EQ(queue.peek_front(readback, Buffer_size), Put_size);

    for (size_t i = 0; i < Put_size; ++i)
    {
        EXPECT_EQ(data[i], readback[i]);
        readback[i] = 0;
    }

    /// peek can happen several time and is non-destructive

    EXPECT_EQ(queue.front_size(), Put_size);
    EXPECT_EQ(queue.peek_front(readback, Buffer_size), Put_size);
    for (size_t i = 0; i < Put_size; ++i)
    {
        EXPECT_EQ(data[i], readback[i]);
        readback[i] = 0;
    }

    EXPECT_EQ(queue.front_size(), Put_size);
    EXPECT_EQ(queue.peek_front(readback, Buffer_size), Put_size);
    for (size_t i = 0; i < Put_size; ++i)
    {
        EXPECT_EQ(data[i], readback[i]);
        readback[i] = 0;
    }
}