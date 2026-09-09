/*
* Copyright (C) 2026 Integrated Detector Electronics AS
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
* @file   Abstract_calibration_test.h
* @author AndersEmilOlsen, IDEAS
* @date   18.06.2026
* @brief
*/


#pragma once
class Instruction_major;

namespace Calibration {
    class Abstract_calibration_test {
        bool is_trace_flag{};

    public:
        virtual ~Abstract_calibration_test() = default;

        [[nodiscard]] virtual bool is_active() const = 0;

        virtual bool start_test(Instruction_major *instruction) = 0;

        virtual void background_process() = 0;

        virtual void print_diag() const = 0;

        void set_trace(bool trace) { is_trace_flag = trace; }

        [[nodiscard]] bool is_trace() const { return is_trace_flag; }
    };
} // Calibration
