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

