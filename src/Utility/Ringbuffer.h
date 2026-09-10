//
// Created by anolsen on 12.09.2019.
//

#pragma once

#include "../Type/Misc_type.h"
#include <cstring>

template<class T, int Buffer_size>
class Ringbuffer {
    uint32_t the_put_count{};
    uint32_t the_pop_count{};
    uint32_t get_empty_count{};
    uint32_t max_fill{};
    uint32_t cnt_overflow{};
    volatile uint32_t read_index{};
    volatile uint32_t write_index{};
    T buf[Buffer_size]{};

public:
    Ringbuffer() = default;

    void initialize() {
        the_put_count = 0;
        the_pop_count = 0;
        get_empty_count = 0;
        max_fill = 0;
        cnt_overflow = 0;
        read_index = 0;
        write_index = 0;

        std::memset(static_cast<void*>(buf), 0, Buffer_size * sizeof(T));
    }

    [[nodiscard]] bool is_corrupted() const {
        return (read_index >= Buffer_size) || (write_index >= Buffer_size);
    }

    bool get(T *data) {
        const bool result = (read_index != write_index);
        if (result) {
            *data = buf[read_index];
            read_index = read_index + 1;
            if (read_index >= Buffer_size) {
                read_index = 0;
            }
            the_pop_count++;
        } else {
            get_empty_count++;
        }
        return result;
    }

    bool put(const T &data) {
        bool result{};
        if (is_getting_filled()) {
            cnt_overflow++;
        } else {
            buf[write_index] = data;
            write_index = write_index + 1;
            if (write_index >= Buffer_size) {
                write_index = 0;
            }
            the_put_count++;
            result = true;
        }
        return result;
    }

    bool put(const T *data, uint32_t count) {
        bool result{};
        if (count >= get_free()) {
            cnt_overflow++;
        } else {
            while (count--) {
                buf[write_index] = *data++;
                write_index = write_index + 1;
                if (write_index >= Buffer_size) {
                    write_index = 0;
                }
                the_put_count++;
            }
            result = true;
        }
        return result;
    }

    bool for_each(Optional_user user, Optional_func func) {
        bool result{};
        T data;
        if (func != nullptr) {
            while (get(&data)) {
                if (func(user, data)) {
                    result = true;
                }
            }
        }
        return result;
    }

    bool is_getting_filled() {
        uint32_t fill = get_fill();
        if (fill > max_fill) { max_fill = fill; }
        return fill > (Buffer_size - 3u);
    }

    [[nodiscard]] bool is_getting_filled_const() const {
        return get_fill() > (Buffer_size - 3u);
    }

    [[nodiscard]] uint32_t get_free() const {
        if (write_index >= read_index) {
            return Buffer_size + read_index - write_index;
        } else {
            return read_index - write_index;
        }
    }

    [[nodiscard]] uint32_t get_fill() const {
        if (write_index >= read_index) {
            return write_index - read_index;
        } else {
            return (Buffer_size + write_index) - read_index;
        }
    }

    bool is_empty() const { return (read_index == write_index); }

    uint32_t get_size() const { return Buffer_size; }

    uint32_t get_put_count() const { return the_put_count; }

    uint32_t get_overflow_count() const { return cnt_overflow; }

    uint32_t get_max_fill() const { return max_fill; }

    void clear_max_fill() { max_fill = 0; }

};
