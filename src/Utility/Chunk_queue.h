//
// Created by AndersEmilOlsen on 01.12.2023.
//

#ifndef TARGET_UTILITY_LIB_CHUNK_QUEUE_H
#define TARGET_UTILITY_LIB_CHUNK_QUEUE_H

#include <cstdint>
#include <type_traits>
#include <algorithm>
#include "Ringbuffer.h"
#include "IChunk_queue.h"

namespace Utility {

    template<size_t Queue_size>
    class Chunk_queue : public IChunk_queue
    {
    public:
        Chunk_queue()
                : the_count{}, the_queue{} {}

        explicit Chunk_queue(uint32_t force_constructor_wo_initialization)
                : the_queue(force_constructor_wo_initialization) {}


        using size_type_size = std::integral_constant<uint8_t, 4U>;
        /*
        bool put(uint32_t *data, uint32_t size) {
            auto success = (the_queue.get_free() > (size + 1)) && the_queue.put(size) && the_queue.put(data, size);
            if (success) { the_count += 1; }
            return success;
        }

        bool get(uint32_t *data, uint32_t size, uint32_t &fill) {
            auto success =
                    (the_queue.get_fill() > 0) && the_queue.get(&fill) && (fill < size) &&
                            the_queue.get_exact(data, fill);
            if (success) { the_count -= 1; } else { fill = 0; }
            return success;
        }
         */

        void initialize() final
        {
            the_count = 0U;
            the_queue.initialize();
        }

        size_t chunk_count() const final { return the_count; }

        bool empty() const final { return the_count == 0U; }

        uint32_t size() const final { return the_queue.get_fill(); }

        uint32_t available() const final { return the_queue.get_free(); }

        uint32_t capacity() const final { return Queue_size; }

        bool is_corrupted() const final
        {
            return the_queue.is_corrupted() ||
                   ((0U == the_count) && (0U != size())) ||
                   ((0U != the_count) && (0U == size()));
        }


        bool push_back(const uint8_t *data, size_t n) final
        {
            bool result = the_queue.get_free() >= (n + size_type_size::value);
            if (result)
            {
                uint8_t size[size_type_size::value]{(uint8_t)((n >> 24U) & 0xFFU),
                                                    (uint8_t)((n >> 16U) & 0xFFU),
                                                    (uint8_t)((n >> 8U) & 0xFFU),
                                                    (uint8_t)(n & 0xFFU)};
                result = the_queue.put(size, size_type_size::value);

                if (result)
                {
                    result = the_queue.put(data, n);
                    the_count++;
                }
            }
            return result;
        }


        size_t front_size() final
        {
            size_t size{0U};
            if (!empty())
            {
                uint8_t n[size_type_size::value]{};
                bool result = the_queue.peek(n, size_type_size::value);

                if (result)
                {
                    size = (n[0] << 24U) | (n[1] << 16U) | (n[2] << 8U) | n[3];
                }
            }
            return size;
        }



        size_t pop_front(uint8_t *data, size_t capacity) final
        {
            size_t size{0U};
            if (!empty())
            {
                uint8_t n[size_type_size::value]{};
                bool result = the_queue.get_exact(n, size_type_size::value);

                if (result)
                {
                    size = (n[0] << 24U) | (n[1] << 16U) | (n[2] << 8U) | n[3];
                    result = (capacity >= size);
                }
                else
                {
                    size = 0U;
                }

                if (result)
                {
                    (void) the_queue.get_exact(data, size);
                    the_count--;
                }
                else
                {
                    size = 0U;
                    // flip MSB/LSB to push front correctly
                    std::swap(n[0], n[3]);
                    std::swap(n[1], n[2]);
                    (void) the_queue.put_front(n, size_type_size::value);
                }
            }
            return size;
        }


        size_t peek_front(uint8_t *data, size_t capacity) final
        {
            size_t size{0U};
            if (!empty())
            {
                uint8_t n[size_type_size::value]{};
                bool result = the_queue.get_exact(n, size_type_size::value);

                if (result)
                {
                    size = (n[0] << 24U) | (n[1] << 16U) | (n[2] << 8U) | n[3];
                    size = the_queue.peek(data, (std::min<size_t>)(capacity, size));

                    // flip MSB/LSB to push front correctly
                    std::swap(n[0], n[3]);
                    std::swap(n[1], n[2]);
                    (void) the_queue.put_front(n, size_type_size::value);
                }
                else
                {
                    size = 0U;
                }
            }
            return size;
        }

        Ringbuffer<uint8_t, Queue_size> &queue() { return the_queue; }
        const Ringbuffer<uint8_t, Queue_size> &queue() const { return the_queue; }

    private:
        /// @warning DO NOT DEFAULT INITIALIZE MEMBERS! ALLOW PERSISTENCE BETWEEN REBOOTS
        size_t the_count;
        Ringbuffer<uint8_t, Queue_size> the_queue;
    };

} // Utility

#endif //TARGET_UTILITY_LIB_CHUNK_QUEUE_H
