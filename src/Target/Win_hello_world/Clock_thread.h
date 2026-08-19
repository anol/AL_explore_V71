#pragma once


#include <ctime>
#include <chrono>
#include <thread>
#ifdef __unix__
#   include <pthread.h>
#endif


namespace Windows {
    class Clock_thread {
     public:
        Clock_thread() = default;

        void run();

    private:
        void thread();

        static ::std::tm time() {
            return to_tm(::std::time(nullptr));
        }

        static ::std::tm to_tm(::std::time_t t) {
            ::std::tm date{};
            localtime_s(&date, &t);
            return date;
        }

        static ::std::time_t to_time_t(const ::std::tm &tm) {
            ::std::tm cpy{};
            memcpy(&cpy, &tm, sizeof(std::tm));
            return mktime(&cpy);
        }

        std::time_t now_offset_sec{};
    };
}
