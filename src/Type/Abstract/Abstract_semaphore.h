#pragma once
namespace Abstract {
    class Abstract_semaphore {
    public:
        virtual ~Abstract_semaphore() = default;

        virtual bool take() = 0;

        virtual bool give() = 0;
    };
} // Abstract
