#define xuser mixc::powerful_cat
#include"chrono/date.hpp"
#include"chrono/datetime.hpp"
#include"chrono/now.hpp"
#include"chrono/time.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

xinit(xuser::the_main){
    using namespace xuser;
    counter demo = 0;

    // 演示：获取时间
    {
        xhint(demo);

        // 得到当前时间
        time<> the_time = now;

        xhint(the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
    }

    // 演示：获取日期
    {
        xhint(demo);

        // 得到当前日期
        date<> the_date = now;

        xhint(the_date.year(), the_date.month(), the_date.day());
    }

    // 演示：获取日期时间
    {
        xhint(demo);

        // 得到当前日期时间
        datetime<> d    = now;

        xhint(d.year(), d.month(), d.day(), d.hour(), d.minute(), d.second(), d.milisecond());
    }

    // 演示：错误用法
    {
        xhint(demo);

        // 直接使用 now.date() / now.time() / now.datetime() 的方式是不正确的
        // 因为存在如下情况：
        // 可能前一秒是 2020/12/31 23:59:59
        // 而你刚好读到 2020 年，然后日期变成了 2021/1/1 00:00:00，当你继续用 now 获取时间属性时
        // 结果就变成了 2020/1/1 00:00:00
        // 当然这样类似的问题都是需要注意的，最好一次性获取所有的属性
        xhint(now.date().year(), now.date().month(), now.date().day());
    }
};
