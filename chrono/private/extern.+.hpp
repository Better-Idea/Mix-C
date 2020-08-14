#ifdef xuser
    #undef xuser
#endif

#include"configure.hpp"
#include"chrono/+.hpp"

#if xis_windows
    #include<windows.h>

    namespace mixc::chrono_date::origin{
        date date::now(){
            SYSTEMTIME t;
            GetLocalTime(& t);
            return date(t.wYear, t.wMonth, t.wDay);
        }
    }

    namespace mixc::chrono_time::origin{
        time time::now(){
            SYSTEMTIME t;
            GetLocalTime(& t);
            return time(t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        }
    }

    namespace mixc::chrono_datetime::origin{
        datetime datetime::now(){
            SYSTEMTIME t;
            GetLocalTime(& t);
            return datetime(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        }
    }
#endif

#if xis_linux
    #include<time.h>
    #include<sys/time.h>
    
    namespace{
        inline auto time_now(){
            timezone z;
            timeval  t;
            gettimeofday(& t, & z);
            t.tv_sec -= z.tz_minuteswest * 60.
            return t;
        }

        inline auto calc_date(timeval t){
            using namespace mixc::chrono_date::origin;
            return date(1970, 1, 1) + day_t(t.tv_sec / (3600 * 24));
        }

        inline auto calc_time(timeval t){
            using namespace mixc::chrono_time::origin;
            auto s    = t.tv_sec % 60;
            auto m    = t.tv_sec / 60 % 60;
            auto h    = t.tv_sec / 3600 % 24;
            return time(h, m, s, t.tv_usec / 1000);
        }
    }

    namespace mixc::chrono_date::origin{
        date date::now(){
            return calc_date(
                time_now()
            );
        }
    }

    namespace mixc::chrono_time::origin{
        time time::now(){
            return calc_time(
                time_now()
            );
        }
    }

    namespace mixc::chrono_datetime::origin{
        datetime datetime::now(){
            auto t = time_now();
            return datetime(
                calc_date(t),
                calc_time(t)
            );
        }
    }
#endif
