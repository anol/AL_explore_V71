/**
 * @file   SimpleBuffer.h
 * @author ChristophePlaissy, IDEAS
 * @date   03/11/2022
 * @brief  
 */

#include <cstdint>
#include <array>

#ifndef BASELINE_EM_SIMPLEBUFFER_H
#define BASELINE_EM_SIMPLEBUFFER_H

/**
 * @class SimpleBuffer
 * @brief Holds elements in an std::array
 * @tparam T type
 * @tparam Capacity array capacity
 * @note Simplified Backbone 3.0.8 :: Buffer class
 */
template <class T, uint32_t Capacity>
class Simple_buffer
{
public:
    using value_type             = T;
    using pointer                = value_type *;
    using const_pointer          = value_type *;
    using reference              = value_type &;
    using const_reference        = const value_type &;
    using array                  = std::array<T, Capacity>;
    using iterator               = typename array::iterator;
    using const_iterator         = typename array::const_iterator;
    using reverse_iterator       = typename array::reverse_iterator;
    using const_reverse_iterator = typename array::const_reverse_iterator;

    enum { BufferCapacity = Capacity };

    Simple_buffer() = default;
    Simple_buffer(const Simple_buffer &) = default;
    Simple_buffer &operator=(const Simple_buffer &) = default;
    explicit Simple_buffer(uint32_t reserved) { reserve(reserved); }
    ~Simple_buffer() { clear(); }

    reference operator[](uint32_t index) { return the_array[index]; }
    const_reference operator[](uint32_t index) const { return the_array[index]; }
    reference at(uint32_t index) { return the_array.at(index); }
    const_reference at(uint32_t index) const { return the_array.at(index); }

    iterator begin() { return iterator(the_array.data()); }
    const_iterator begin() const { return cbegin(); }
    const_iterator cbegin() const { return const_iterator(the_array.data()); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return crbegin(); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }

    iterator end() { return iterator(the_array.data() + the_size); }
    const_iterator end() const { return cend(); }
    const_iterator cend() const { return const_iterator(the_array.data() + the_size); }

    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return crend(); }
    const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

    uint32_t capacity() const { return Capacity; }
    uint32_t size() const { return the_size; }
    uint32_t available() const { return capacity() - size(); }
    bool full() const { return the_size == Capacity; }
    bool empty() const { return 0 == the_size; }
    void clear() { the_size = 0; } ///< @brief does not destroy contents

    /**
     * @fn reserve
     * @brief pre-allocates a number of elements in buffer, using the element default's constructor. Does not reallocate existing objects.
     * @param size
     */
    void reserve(uint32_t size)
    {
        uint32_t i = the_size;
        for (; (i < Capacity) && (i < size); ++i)
        {
            the_array[i] = value_type();
        }
        the_size = i;
    }

    reference front() { return the_array[0]; }
    const_reference front() const { return the_array[0]; }
    reference back() { return the_array[the_size - 1]; }
    const_reference back() const { return the_array[the_size - 1]; }

    Simple_buffer &operator+=(const_reference obj)
    {
        push_back(obj);
        return *this;
    }

    /**
     * @fn push_back
     * @brief pushes an element to the end of the buffer
     * @param obj element to push back
     * @return true if element was pushed, false if full before push action
     */
    bool push_back(const_reference obj)
    {
        if (!full())
        {
            the_array[the_size] = obj;
            the_size++;
            return true;
        }
        return false;
    }

    /**
     * @fn pop_back
     * @brief reduces the buffer size by ignoring the last element
     * @note does not destroy, simply ignores last element making it out of range
     */
    void pop_back()
    {
        if (!empty()) the_size--;
    }


private:
    array the_array{};
    uint32_t the_size{};
};

#endif //BASELINE_EM_SIMPLEBUFFER_H
