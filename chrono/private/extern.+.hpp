#undef  xuser
#include"configure/switch.hpp"
#include"chrono/now.hpp"

#if xis_windows
    #include<windows.h>

    namespace mixc::chrono_now{
        inc::date<> now_t::date() {
            SYSTEMTIME t;
            GetLocalTime(& t);
            return inc::date<>(t.wYear, t.wMonth, t.wDay);
        }

        inc::datetime<> now_t::datetime() {
            SYSTEMTIME t;
            GetLocalTime(& t);
            return inc::datetime<>(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        }

        inc::time<> now_t::time() {
            SYSTEMTIME t;
            GetLocalTime(& t);
            return inc::time<>(t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        }
    }
#endif

#if xis_linux
    #include<time.h>
    #include<sys/time.h>

    namespace mixc::chrono_now{
        inline auto time_now(){
            struct ::timezone z;
            struct ::timeval  t;
            gettimeofday(& t, & z);
            t.tv_sec -= z.tz_minuteswest * 60;
            return t;
        }

        inline auto calc_date(timeval t){
            return 
                inc::date<>(1970, 1, 1) + 
                inc::day(t.tv_sec / (3600 * 24));
        }

        inline auto calc_time(timeval t){
            auto s    = t.tv_sec % 60;
            auto m    = t.tv_sec / 60 % 60;
            auto h    = t.tv_sec / 3600 % 24;
            return inc::time<>(h, m, s, t.tv_usec / 1000);
        }

        inc::date<> now_t::date() {
            return calc_date(
                time_now()
            );
        }

        inc::datetime<> now_t::datetime() {
            auto t = time_now();
            return inc::datetime<>(
                calc_date(t),
                calc_time(t)
            );
        }

        inc::time<> now_t::time() {
            return calc_time(
                time_now()
            );
        }
    }
#endif
