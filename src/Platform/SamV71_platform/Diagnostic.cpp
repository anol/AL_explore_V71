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

#include <cstdio>

#include "Diagnostic.h"

#include "Status_code.h"

namespace Error_handling
{
    static void error_prolog()
    {
        printf( "<< ERROR: ");
    }

    static void error_epilog()
    {
        printf(". >>\r\n");
    }

    void information(const char* message)
    {
        printf("%s\r\n", message);
    }

    void information(const char* message, uint32_t diag1)
    {
        printf(message, diag1);
        printf("\r\n");
    }

    void information(const char* message, const char* diag1)
    {
        printf(message, diag1);
        printf("\r\n");
    }

    void information(const char* message, uint32_t diag1, uint32_t diag2)
    {
        printf(message, diag1, diag2);
        printf("\r\n");
    }

    void information(const char* message, uint32_t diag1, uint32_t diag2, const char* diag3)
    {
        printf(message, diag1, diag2, diag3);
        printf("\r\n");
    }

    void information(const char* message, const char* diag1, uint32_t diag2, uint32_t diag3)
    {
        printf(message, diag1, diag2, diag3);
        printf("\r\n");
    }

    void information(const char* message,
                     uint32_t diag1, uint32_t diag2, uint32_t diag3)
    {
        printf(message, diag1, diag2, diag3);
        printf("\r\n");
    }

    void information(const char* message,
                     void* diag1, uint32_t diag2, uint32_t diag3)
    {
        printf(message, diag1, diag2, diag3);
        printf("\r\n");
    }

    void information(const char* message,
                     uint32_t diag1, uint32_t diag2, uint32_t diag3, uint32_t diag4)
    {
        printf(message, diag1, diag2, diag3, diag4);
        printf("\r\n");
    }

    void information(const char* message,
                     uint32_t diag1, uint32_t diag2, uint32_t diag3,
                     uint32_t diag4, uint32_t diag5, uint32_t diag6)
    {
        printf(message, diag1, diag2, diag3, diag4, diag5, diag6);
        printf("\r\n");
    }

    void information(const char* message,
                     uint32_t diag1, uint32_t diag2, uint32_t diag3,
                     uint32_t diag4, uint32_t diag5, uint32_t diag6, uint32_t diag7)
    {
        printf(message, diag1, diag2, diag3, diag4, diag5, diag6, diag7);
        printf("\r\n");
    }

    void information(const char* message, uint32_t diag1, const char* diag2)
    {
        printf(message, diag1, diag2);
        printf("\r\n");
    }

    void information(const char* message, const char* diag1, const char* diag2)
    {
        printf(message, diag1, diag2);
        printf("\r\n");
    }

    void error_message(const char* message)
    {
        error_prolog();
        printf(message);
        error_epilog();
    }

    void error_message(const char* message, uint32_t diag1)
    {
        error_prolog();
        printf(message, diag1);
        error_epilog();
    }

    void error_message(const char* message, const char* diag1)
    {
        error_prolog();
        printf(message, diag1);
        error_epilog();
    }

    void error_message(const char* message, uint32_t diag1, uint32_t diag2)
    {
        error_prolog();
        printf(message, diag1, diag2);
        error_epilog();
    }

    void error_message(const char* message, uint32_t diag1, const char* diag2)
    {
        error_prolog();
        printf(message, diag1, diag2);
        error_epilog();
    }

    void error_message(const char* message, const char* diag1, const char* diag2, const char* diag3)
    {
        error_prolog();
        printf(message, diag1, diag2, diag3);
        error_epilog();
    }

    void error_code(const Status_code& code, const char* diag)
    {
        error_prolog();
        printf("%s", diag);
        error_epilog();
        error_code(code);
    }

    void error_code(const Status_code& code, const char* diag, uint32_t diag1)
    {
        error_prolog();
        printf(diag, diag1);
        error_epilog();
        error_code(code);
    }

    void error_code(const Status_code& code, const char* diag, uint32_t diag1, uint32_t diag2)
    {
        error_prolog();
        printf(diag, diag1, diag2);
        error_epilog();
        error_code(code);
    }

    void error_code(const Status_code& code, const char* diag, const char* diag1)
    {
        error_prolog();
        printf(diag, diag1);
        error_epilog();
        error_code(code);
    }

    void error_code(uint32_t code)
    {
        printf("ERROR=%lu IN MODULE %lX.\r\n", (code & 0xFFFF), (code >> 16));
    }

    void error_code(uint32_t code, const char* diag)
    {
        error_prolog();
        printf("%s", diag);
        error_epilog();
        error_code(code);
    }

    void error_code(uint32_t code, const char* diag, uint32_t diag1)
    {
        error_prolog();
        printf(diag, diag1);
        error_epilog();
        error_code(code);
    }

    void error_code(uint32_t code, const char* diag, const char* diag1)
    {
        error_prolog();
        printf(diag, diag1);
        error_epilog();
        error_code(code);
    }
} // Diagnostic
