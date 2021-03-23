#define xuser mixc::powerful_cat
#include"chrono/day.hpp"
#include"chrono/time.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

int run(){
    using namespace xuser;
    counter demo = 0;

    // 演示：创建时间
    {
        xhint(demo);
        
        // 创建时间
        time<> the_time;

        // 设置时间
        the_time.hour(7).minute(22).second(40).milisecond(123);

        // 打印时间
        xhint(the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
    }

    // 演示：时间校验
    {
        xhint(demo);

        // 创建时间
        time<> the_time;

        // hour 可以大于 23
        the_time.hour(24).minute(zero).second(zero).milisecond(zero);
        xhint(the_time.is_valid(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());

        // 但对于 24 小时制的时间 hour 不能大于 23
        xhint(the_time.is_valid_24h_clock(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
        the_time.hour(23);
        xhint(the_time.is_valid_24h_clock(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());

        // minute 不能大于 59
        the_time.hour(zero).minute(59).second(zero).milisecond(zero);
        xhint(the_time.is_valid(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
        the_time.minute(60);
        xhint(the_time.is_valid(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());

        // second 不能大于 59
        the_time.minute(59).second(59);
        xhint(the_time.is_valid(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
        the_time.second(60);
        xhint(the_time.is_valid(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());

        // 当允许润秒时 second 最大可以为 60
        xhint(the_time.is_valid(allow_leap_second), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());

        // 但不能超过 60
        the_time.second(61);
        xhint(the_time.is_valid(allow_leap_second), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());

        // milisecond 不能大于 999
        the_time.second(zero).milisecond(999);
        xhint(the_time.is_valid(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
        the_time.milisecond(1000);
        xhint(the_time.is_valid(), the_time.hour(), the_time.minute(), the_time.second(), the_time.milisecond());
    }

    // 演示：计算总毫秒数
    {
        xhint(demo);

        // 创建时间
        time<>  the_time;
        u64     total_ms;
        the_time.hour(1).minute(2).second(3).milisecond(4);

        total_ms        = the_time.total_milisecond();
        xhint(total_ms);
    }

    // 演示：时间的比较
    {
        xhint(demo);

        time<> a;
        time<> b;
        a.hour(20);
        b.hour(21);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.hour(20).minute(2);
        b.hour(20).minute(1);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.hour(20).minute(1).second(20);
        b.hour(20).minute(1).second(21);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.hour(20).minute(1).second(21);
        b.hour(20).minute(1).second(21);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.hour(20).minute(1).second(21).milisecond(998);
        b.hour(20).minute(1).second(21).milisecond(999);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.hour(21).minute(1).second(21).milisecond(998);
        b.hour(20).minute(2).second(21).milisecond(999);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);
    }
    return 0;
}
