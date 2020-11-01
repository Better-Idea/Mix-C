#ifndef xpack_chrono_now
#define xpack_chrono_now
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_now::inc
#include"chrono/date.hpp"
#include"chrono/datetime.hpp"
#include"chrono/time.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_now{
    struct now_t{
        enum{ im_now_t = 1 };
        constexpr now_t(){}
        static inc::date<>      date();
        static inc::datetime<>  datetime();
        static inc::time<>      time();
    };

    constexpr now_t now{};
}

#endif

xexport(mixc::chrono_now::now)
