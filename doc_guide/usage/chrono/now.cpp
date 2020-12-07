#define xuser mixc::powerful_cat
#include"chrono/date.hpp"
#include"chrono/datetime.hpp"
#include"chrono/now.hpp"
#include"chrono/time.hpp"
#include"mixc.hpp"

namespace xuser{
    void demo(){
        uxx demo = 0;

        // 演示：获取日期
        {
            xhint(demo);
            demo           += 1;

            // 得到当前日期
            date<> the_date = now;

            xhint(the_date.year(), the_date.month(), the_date.day());
        }

        // 演示：获取时间
        {
            xhint(demo);
            demo           += 1;

            // 得到当前时间
            time<> the_time = now;

            xhint(the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
        }

        // 演示：获取日期时间
        {
            xhint(demo);
            demo           += 1;

            // 得到当前日期时间
            datetime<> d    = now;

            xhint(d.year(), d.month(), d.day(), d.hour(), d.minute(), d.second(), d.milisecond());
        }
    }
}

int main(){
    xuser::demo();
    return 0;
}