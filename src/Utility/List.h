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
 * \file   List.h
 * \author cplaissy, IDEAS
 * \date   2021-05-12
 * \brief  Similar to Ringbuffer, but aims at providing access to individual members and/or modify then from an external process
 */


#ifndef TARGET_EVAL_RH71_LIST_H
#define TARGET_EVAL_RH71_LIST_H

#include "Utility_types.h"

/**
 * @brief List class to hold items to be held, randomly accessed and sort.
 *        It is implemented using three buffers, all of the same size:
 *        1. one of type T, it is the element buffer
 *        2. one that holds the current status for each index in element buffer (whether it is a free spot or an inserted element)
 *        3. and one that holds pointers to elements of the element buffer.
 * @tparam T
 * @tparam List_size
 */
template <typename T, int List_size>
class List
{
private:
    struct Item
    {
        enum State : bool
        {
            Free     = true,
            Inserted = false,
        };
        typedef T* Pointer;
    };


public:
    List<T, List_size>()
    {
        for (auto &s: state_list) s = Item::Free;
    }

    inline uint32_t get_size() const           { return the_size; }
    inline uint32_t get_capacity() const       { return List_size; }
    inline bool is_empty() const               { return (0 == the_size); }
    inline bool is_full() const                { return (the_size >= get_capacity()); }
    inline uint32_t get_put_count() const      { return the_put_count; }
    inline uint32_t get_overflow_count() const { return cnt_overflow; }
    inline uint32_t get_max_fill() const       { return max_fill; }

    /**
     * @brief Pushes an element at the end of the list
     * @param element
     * @return pointer to element in list if pushed, nullptr otherwise
     */
    T* push(const T& element)
    {
        T* result = nullptr;
        if (!is_full())
        {
            find_next_free();
            if ((_next_free < List_size) && (Item::Free == state_list[_next_free]))
            {
                state_list[_next_free] = Item::Inserted;
                buffer[_next_free]     = element;
                ptr_list[the_size]     = &buffer[_next_free];
                result                 = ptr_list[the_size];
                the_size++;
                _next_free++;
                the_put_count++;
                if (the_size > max_fill) max_fill = the_size;
            }
            else cnt_overflow++;
        }
        else cnt_overflow++;
        return result;
    }

    /**
     * @brief returns a pointer to an element in list
     * @param index - index in list of element to get
     * @return valid pointer if valid index, nullptr otherwise
     */
    T* get(uint32_t index)
    {
        if (the_size > index)
        {
            uint32_t buf_index = get_buffer_index(ptr_list[index]);
            if ((List_size > buf_index) && (Item::Inserted == state_list[buf_index])) return ptr_list[index];
        }
        return nullptr;
    }

    /**
     * @brief Function type to compare elements of the list:
     *        with find: return true to match -> find element that matches given condition
     *        with sort: return false to swap two elements (i.e. return lhs < rhs)
     */
    typedef bool (*compare_f)(const T& lhs, const T& rhs);

    /**
     * @brief Finds an element in list
     * @param lhs - element to find, provided by external process
     * @param func - comparison function to compare elements in list with lhs
     * @return pointer to first element in list that func found to compare with lhs, nullptr if none
     */
    T* find(const T& lhs, compare_f func)
    {
        if ((nullptr != func) && !is_empty())
        {
            for (uint32_t i = 0; i < the_size; ++i)
            {
                if (func(lhs, *ptr_list[i]))
                {
                    return ptr_list[i];
                }
            }
        }
        return nullptr;
    }

    /**
     * @brief Sorts elements of list given a comparison function
     * @param func - comparison function to sort elements in list
     */
    void sort(compare_f func)
    {
        // only sort out pointers to elements, elements stay where they are
        if ((nullptr != func) && (1 < the_size))
        {
            typename Item::Pointer lhs;
            typename Item::Pointer rhs;
            typename Item::Pointer tmp;
            bool increment;
            for (uint32_t i = 0; (i + 1) < the_size; increment? ++i : --i)
            {
                increment = true;
                lhs = ptr_list[i];
                rhs = ptr_list[i + 1];
                if (!func(*lhs, *rhs))
                {
                    tmp             = lhs;
                    ptr_list[i]     = rhs;
                    ptr_list[i + 1] = tmp;
                    if (0 < i) increment = false;
                }
            }
        }
    }

    /**
     * @brief Enumeration type to provide some level of automated handling inside for_each function.
     * 'continue' and 'break' can be or'ed with 'pop'
     */
    enum for_each_ret
    {
        for_each_continue = 0,
        for_each_break    = 1,
        for_each_pop      = 2,
    };

    /**
     * @brief Function type to handle each listed item separately in the order they are listed
     */
    typedef for_each_ret (*peek_f)(void* user, T& element, void* context);

    bool for_each(void* user, peek_f func, void *context = nullptr)
    {
        if ((nullptr != func) && !is_empty())
        {
            for_each_ret result;
            bool increment;
            for (uint32_t i = 0; i < the_size; increment? ++i : (i += 0))
            {
                increment = true;
                result = func(user, *ptr_list[i], context);

                if (for_each_pop & result)
                {
                    pop(ptr_list[i]);
                    increment = false; // current element just pop'd, continue at same index on next loop iteration
                }
                if (for_each_break & result) break;
            }
            return true;
        }
        return false;
    }


    bool pop(T* ptr)
    {
        bool result = false;
        if (!is_empty() && (nullptr != ptr))
        {
            uint32_t index = get_buffer_index(ptr);
            if (List_size > index)
            {
                buffer[index] = T(); // reset element in buffer
                state_list[index] = Item::Free; // update corresponding element state
                if (index < _next_free) _next_free = index;
                for (uint32_t i = 0; i < the_size; ++i) // update pointer list
                {
                    if (ptr == ptr_list[i])
                    {
                        for (uint32_t j = (i + 1); j < the_size; ++j) ptr_list[j - 1] = ptr_list[j];
                        the_size--;
                        ptr_list[the_size] = nullptr;
                        break;
                    }
                }
                result = true;
            }
        }
        return result;
    }


    void clear() {
        the_size = 0;
        _next_free = 0;
        for (auto &s: state_list) s = Item::Free;
    }

private:
    T buffer[List_size]{};
    typename Item::State state_list[List_size] = {Item::Free};
    typename Item::Pointer ptr_list[List_size] = {nullptr};
    uint32_t the_size{};
    uint32_t _next_free{};
    uint32_t the_put_count{};
    uint32_t cnt_overflow{};
    uint32_t max_fill{};


    uint32_t get_buffer_index(T *ptr) const
    {
        if (ptr >= buffer) return (ptr - buffer);
        return List_size;
    }

    void find_next_free()
    {
        if ((_next_free >= List_size) || (Item::Free != state_list[_next_free]))
        {
            for (uint32_t i = 0U; i < List_size; ++i)
            {
                //if (i >= List_size) i = 0;
                if (Item::Free == state_list[i])
                {
                    _next_free = i;
                    break;
                }
            }
        }
    }
};

#endif //TARGET_EVAL_RH71_LIST_H
