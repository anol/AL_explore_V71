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
 *
 */

/**
 * \date   IDEAS/28.09.2020/aeols
 * \brief
 */

#pragma once

#include <cstdint>

#include "Error_code.h"

namespace Error_handling {

    using namespace Dictionary;

    void information(const char *message);

    void information(const char *message, uint32_t diag1);

    void information(const char *message, const char *diag1);

    void information(const char *message, uint32_t diag1, uint32_t diag2);

    void information(const char *message, uint32_t diag1, const char *diag2);

    void information(const char *message, const char *diag1, const char *diag2);

    void information(const char *message, uint32_t diag1, uint32_t diag2, const char *diag3);

    void information(const char *message, const char *diag1, uint32_t diag2, uint32_t diag3);

    void information(const char *message, uint32_t diag1, uint32_t diag2, uint32_t diag3);

    void information(const char *message, void *diag1, uint32_t diag2, uint32_t diag3);

    void information(const char *message, uint32_t diag1, uint32_t diag2, uint32_t diag3, uint32_t diag4);

    void information(const char *message,
                     uint32_t diag1, uint32_t diag2, uint32_t diag3,
                     uint32_t diag4, uint32_t diag5, uint32_t diag6);

    void information(const char *message,
                     uint32_t diag1, uint32_t diag2, uint32_t diag3,
                     uint32_t diag4, uint32_t diag5, uint32_t diag6, uint32_t diag7);

    void error_message(const char *message);

    void error_message(const char *message, uint32_t diag1);

    void error_message(const char *message, const char *diag1);

    void error_message(const char *message, uint32_t diag1, uint32_t diag2);

    void error_message(const char *message, uint32_t diag1, const char *diag2);

    void error_message(const char *message, const char *diag1, const char *diag2, const char *diag3);

    void error_code(uint32_t code);

    void error_code(uint32_t code, const char *diag);

    void error_code(uint32_t code, const char *diag, uint32_t diag1);

    void error_code(uint32_t code, const char *diag, const char *diag1);

    void error_code(const Error_code &);

    void error_code(const Error_code &, const char *);

    void error_code(const Error_code &, const char *, uint32_t);

    void error_code(const Error_code &, const char *, uint32_t, uint32_t);

    void error_code(const Error_code &, const char *, const char *);
};
