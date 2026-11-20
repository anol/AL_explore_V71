
#include "Clock_thread.h"

namespace Windows
{
    void Clock_thread::run(){
        now_offset_sec = to_time_t(time());
        std::thread thr = std::thread(&Clock_thread::thread, this);
        thr.detach();
    }

    void Clock_thread::thread()  {
        while (true) {
            auto now = to_time_t(time());
            if (now != now_offset_sec) {
                now_offset_sec = now;
            }
        }
    }
}
