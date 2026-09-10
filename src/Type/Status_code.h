//
// Created by aeols on 2026-09-10.
//

#pragma once

enum class Status_value { Success, Failed };

class Status_code
{
    Status_value the_status;

public:
    Status_code() : the_status(Status_value::Failed)
    {
    }

    explicit Status_code(const Status_value status) : the_status(status)
    {
    }

    explicit Status_code(const bool status) : the_status(status ? Status_value::Success : Status_value::Failed)
    {
    }

    static Status_code Success() { return Status_code(Status_value::Success); }
    static Status_code Failure() { return Status_code(Status_value::Failed); }

    void set_success() { the_status = Status_value::Success; }
    void set_failed() { the_status = Status_value::Failed; }

    [[nodiscard]] bool success() const { return the_status == Status_value::Success; }
    [[nodiscard]] bool failed() const { return the_status == Status_value::Failed; }
};
