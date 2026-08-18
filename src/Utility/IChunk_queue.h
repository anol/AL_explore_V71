/*
 * Copyright (C) 2020-2025 Integrated Detector Electronics AS
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
 * @file   IChunk_queue.h
 * @author ChristophePlaissy, IDEAS
 * @date   25.02.2025
 * @brief  
 */

#ifndef TARGET_UTILITY_LIB_ICHUNK_QUEUE_H
#define TARGET_UTILITY_LIB_ICHUNK_QUEUE_H

namespace Utility
{
    class IChunk_queue
    {
    public:
        virtual void initialize() = 0;

        virtual size_t chunk_count() const = 0;

        virtual bool empty() const = 0;

        virtual uint32_t size() const = 0;

        virtual uint32_t available() const = 0;

        virtual uint32_t capacity() const = 0;

        virtual bool is_corrupted() const = 0;


        virtual bool push_back(const uint8_t *data, size_t n) = 0;

        virtual size_t front_size() = 0;

        virtual size_t pop_front(uint8_t *data, size_t capacity) = 0;

        virtual size_t peek_front(uint8_t *data, size_t capacity) = 0;
    };
}

#endif //TARGET_UTILITY_LIB_ICHUNK_QUEUE_H
