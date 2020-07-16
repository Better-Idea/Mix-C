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
    
    namespace mixc::chrono_date::origin{
        date date::now(){
            auto t = *localtime(nullptr);
            return date(1900 + t.tm_year, 1 + t.tm_mon, t.tm_mday);
        }
    }

    namespace mixc::chrono_time::origin{
        time time::now(){
            while(true){
                timeval low;
                auto    high = *localtime(nullptr);
                gettimeofday(& low, nullptr);

                if (low.tv_sec == high.tm_sec){
                    return time(high.tm_hour, high.tm_min, high.tm_sec, (low.tv_usec + 10) / 1000);
                }
            }
        }
    }

    namespace mixc::chrono_datetime::origin{
        datetime datetime::now(){
            while(true){
                timeval low;
                auto    high = *localtime(nullptr);
                gettimeofday(& low, nullptr);

                if (low.tv_sec == high.tm_sec){
                    return datetime(1900 + high.tm_year, 1 + high.tm_mon, high.tm_mday, high.tm_hour, high.tm_min, high.tm_sec, (low.tv_usec + 10) / 1000);
                }
            }
        }
    }
#endif

