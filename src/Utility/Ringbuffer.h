//
// Created by anolsen on 12.09.2019.
//

#ifndef NORM_FW_RINGBUFFER_H
#define NORM_FW_RINGBUFFER_H

#include "Utility_types.h"
#include <cstring>

template<class T, int Buffer_size>
class Ringbuffer {
public:
    Ringbuffer() : the_put_count(0),
                   the_pop_count(0),
                   get_empty_count(0),
                   max_fill(0),
                   cnt_overflow(0),
                   read_index(0),
                   write_index(0) {
    }

    explicit Ringbuffer(uint32_t force_constructor_wo_initialization) {
        get_empty_count = force_constructor_wo_initialization;
    }

    void initialize() {
        the_put_count = 0;
        the_pop_count = 0;
        get_empty_count = 0;
        max_fill = 0;
        cnt_overflow = 0;
        read_index = 0;
        write_index = 0;

        std::memset((void *) buf, 0, Buffer_size * sizeof(T));
    }

    bool is_corrupted() const {
        return (read_index >= Buffer_size) || (write_index >= Buffer_size);
    }

    T &at(uint32_t index) {
        if (index < get_fill()) {
            return buf[get_internal_index(index)];
        }
        // always returns a valid reference, even if empty, user must assess is_empty() and if index is within range
        else return buf[read_index];
    }

    const T &at(uint32_t index) const {
        if (index < get_fill()) {
            return buf[get_internal_index(index)];
        }
        // always returns a valid reference, even if empty, user must assess is_empty() and if index is within range
        else return buf[read_index];
    }

    T &back() {
        uint32_t index = (write_index == 0) ? (Buffer_size - 1) : (write_index - 1);
        return buf[index]; // always returns a valid reference, even if empty, user must assess is_empty()
    }

    const T &back() const {
        uint32_t index = (write_index == 0) ? (Buffer_size - 1) : (write_index - 1);
        return buf[index]; // always returns a valid reference, even if empty, user must assess is_empty()
    }

    bool peek(T *data) const {
        bool result = (read_index != write_index);
        if (result) {
            *data = buf[read_index];
        }
        return result;
    }

    size_t peek(T *data, size_t n) const {
        auto tmp_read_index = read_index;
        size_t read{0U};
        const size_t max = get_fill();

        while ((tmp_read_index != write_index) && (read < n) && (read < max)) {
            *data = buf[tmp_read_index];
            data++;
            tmp_read_index++;
            read++;
            if (tmp_read_index >= Buffer_size) {
                tmp_read_index = 0;
            }
        }

        return read;
    }

    bool get(T *data) {
        bool result = (read_index != write_index);
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

    bool get_exact(T *data, uint32_t count) {
        bool result{};
        if (count <= get_fill()) {
            while (count--) {
                *data++ = buf[read_index];
                read_index = read_index + 1;
                if (read_index >= Buffer_size) {
                    read_index = 0;
                }
                the_pop_count++;
            }
            result = true;
        } else {
            get_empty_count++;
        }
        return result;
    }

    uint32_t get_upto(T *data, uint32_t size) {
        uint32_t count{};
        auto fill = get_fill();
        if (fill) {
            if (size > fill) {
                size = fill;
            }
            while (size--) {
                count++;
                *data++ = buf[read_index];
                read_index = read_index + 1;
                if (read_index >= Buffer_size) {
                    read_index = 0;
                }
                the_pop_count++;
            }
        }
        return count;
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

    bool put_front(const T &data) {
        bool result{};
        if (is_getting_filled()) {
            cnt_overflow++;
        } else {
            read_index = read_index - 1;
            if (read_index >= Buffer_size) {
                read_index = Buffer_size - 1U;
            }
            buf[read_index] = data;
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


    bool put_front(const T *data, uint32_t count) {
        bool result{};
        if (is_getting_filled()) {
            cnt_overflow++;
        } else {
            while (count--) {
                read_index = read_index - 1;
                if (read_index >= Buffer_size) {
                    read_index = Buffer_size - 1U;
                }
                buf[read_index] = *data++;
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

    bool is_getting_filled_const() const {
        return get_fill() > (Buffer_size - 3u);
    }

    uint32_t get_free() const {
        if (write_index >= read_index) {
            return Buffer_size + read_index - write_index;
        } else {
            return read_index - write_index;
        }
    }

    uint32_t get_fill() const {
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

    uint32_t get_internal_index(uint32_t index) const {
        index = read_index + index;

        while (index >= Buffer_size) index -= Buffer_size;

        if (write_index > read_index) return (index < write_index) ? index : read_index;
        else return ((index >= read_index) || (index < write_index)) ? index : read_index;
    }

private:
    // NOTE: These fields are not auto-initialized - use initialize()!
    // Why? Objects of this kind may be stored in persistent memory.
    uint32_t the_put_count;
    uint32_t the_pop_count;
    uint32_t get_empty_count;
    uint32_t max_fill;
    uint32_t cnt_overflow;
    volatile uint32_t read_index;
    volatile uint32_t write_index;
    T buf[Buffer_size];
};

#endif //NORM_FW_RINGBUFFER_H
