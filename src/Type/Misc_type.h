/*
 * Copyright (C) 2021 Integrated Detector Electronics AS
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
 * \date   IDEAS/09.02.2021/aeols
 * \brief
 */

#ifndef TARGET_BOOT_V71_UTILITY_TYPES_H
#define TARGET_BOOT_V71_UTILITY_TYPES_H

#include <cstdint>

using Milliseconds = uint32_t;
using Microseconds = uint32_t;
using Nanoseconds = uint32_t;
using Frequency = uint32_t;
using Data_size = uint32_t;
using User_data = uint32_t;
using Optional_data = void *;
using Optional_text = const char *;
using Optional_user = void *;
using Optional_func = bool (*)(Optional_user, User_data);
using Optional_reply = bool (*)(Optional_user, Optional_data, User_data);
using Optional_handler = bool (*)(Optional_user, Optional_text, User_data);
using Optional_confirm = bool (*)(Optional_user, Optional_data, bool);

#endif //TARGET_BOOT_V71_UTILITY_TYPES_H
