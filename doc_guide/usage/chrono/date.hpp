#define xuser mixc::powerful_cat
#include"chrono/date.hpp"
#include"chrono/now.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

xinit(xuser::the_main){
    using namespace xuser;
    counter demo = 0;

    // 演示：获取当前日期
    {
        xhint(demo);

        // 获取当前日期
        date<> date_now     = now;

        xhint(date_now.year(), date_now.month(), date_now.day());
    }

    // 演示：推荐的初始化日期方法
    {
        xhint(demo);

        date<> the_date;

        // 通过链式调用设置日期属性
        // 创建日期
        the_date.year(2020).month(12).day(5);

        xhint(the_date.year(), the_date.month(), the_date.day());
    }

    // 演示：判断日期是否合法
    {
        xhint(demo);

        date<> the_date;

        // 通过链式调用设置日期属性
        // 创建日期
        the_date.year(2020).month(12).day(5);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());

        // year 必须大于 0 
        // 该日期不合法
        the_date.year(zero).month(12).day(5);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());

        // month 必须大于 0 
        // 该日期不合法
        the_date.year(2020).month(zero).day(5);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());

        // month 必须小于等于 12 
        // 该日期不合法
        the_date.year(2020).month(13).day(5);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());

        // day 必须大于 0 
        // 该日期不合法
        the_date.year(2020).month(12).day(zero);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());

        // day 必须小于等于当月最大的天数
        // day 溢出
        // 该日期不合法
        the_date.year(2020).month(12).day(32);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());

        // 因为 2020 年是闰年，所以 2 月有 29 天
        // 该日期合法
        the_date.year(2020).month(2).day(29);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());

        // 因为 2019 年是平年，所以 2 月只有 28 天
        // 该日期不合法
        the_date.year(2019).month(2).day(29);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.is_valid());
    }

    // 演示：判断当前年份是否是闰年
    {
        xhint(demo);

        date<> the_date;

        // 2003 不能被 4 整除，所以不是闰年
        xhint(the_date.year(2003).is_leap());

        // 1900 年由于能被 100 整除所以不是闰年
        xhint(the_date.year(1900).is_leap());

        // 2000 年是虽然能被 100 整除，由于也能被 400 整除，所以是闰年
        xhint(the_date.year(2000).is_leap());

        // 2004 年是闰年
        xhint(the_date.year(2004).is_leap());
    }

    // 演示：计算明天对应的日期
    {
        xhint(demo);

        date<> the_date;
        the_date.year(2020).month(2).day(28);

        // 明天是 2020/2/29
        the_date = the_date.tomorrow();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 明天是 2020/3/1
        the_date = the_date.tomorrow();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 明天是 2020/12/31
        the_date.year(2020).month(12).day(30);
        the_date = the_date.tomorrow();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 明天是 2021/1/1
        the_date = the_date.tomorrow();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 注意：
        // year 能表示的范围为[0, 2^23)，所以一直迭代计算 tomorrow 会溢出，如果必要需要做边界处理
    }

    // 演示：计算昨天对应的日期
    {

        xhint(demo);

        date<> the_date;
        the_date.year(2020).month(3).day(2);

        // 昨天是 2020/3/1
        the_date = the_date.yesterday();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 昨天是 2020/2/29
        the_date = the_date.yesterday();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 昨天是 2020/1/1
        the_date.year(2020).month(1).day(2);
        the_date = the_date.yesterday();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 昨天是 2019/12/31
        the_date = the_date.yesterday();
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 注意：
        // year 能表示的范围为[0, 2^23)，所以一直迭代计算 yesterday 会溢出，如果必要需要做边界处理
    }

    // 演示：日期减法
    {

        xhint(demo);

        date<> the_date;
        date<> old_date;

        // 2 天前是 2020/2/29
        the_date.year(2020).month(3).day(2);
        the_date = the_date - day{2};
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 1 年前是 2019/2/28
        the_date = the_date - day{366}; /*2020 年全年 366 天*/
        xhint(the_date.year(), the_date.month(), the_date.day());

        old_date.year(2020).month(1).day(1);
        the_date.year(2021).month(1).day(1);

        // 2020 年一共有 366 天
        day diff = the_date - old_date;
        xhint(diff);
    }

    // 演示：日期加法
    {
        xhint(demo);

        date<> the_date;
        date<> old_date;

        // 27 天后是 2020/2/28
        the_date.year(2020).month(2).day(1);
        the_date = the_date + day{27};
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 1 年后是 2021/2/28
        the_date = the_date + day{366}; /*2020 年全年 366 天*/
        xhint(the_date.year(), the_date.month(), the_date.day());

        // 注意该日期是元年第一天
        // 也就是所谓的第 0 日
        old_date.year(1).month(1).day(1);
        the_date.year(2021).month(1).day(1);

        // 日期的相加就和指针的乘法一样并没有什么太大的意义
        // 我们并没有提供显式的日期之间的加法，而是日期可以转成 day 类型
        // 而 date 实现的是日期加减天数的运算重载
        the_date = the_date + old_date/*隐式转成 day 类型*/;

        // 由于加的是第 0 日，所以日期不变
        xhint(the_date.year(), the_date.month(), the_date.day());
    }

    // 演示：今天是星期几
    {
        xhint(demo);

        date<> today        = now;
        asciis day_of_week;
        asciis lut[]        = {
            "sunday",
            "monday",
            "tuesday",
            "wednesday",
            "thursday",
            "friday",
            "saturday",
        };

        day_of_week         = lut[(uxx)today.day_of_week()];
        xhint(today.year(), today.month(), today.day(), day_of_week);

        // 2020/2/29 是星期六
        today.year(2020).month(2).day(29);
        day_of_week         = lut[(uxx)today.day_of_week()];
        xhint(today.year(), today.month(), today.day(), day_of_week);

        // 2019/2/28 是星期四
        today.year(2019).month(2).day(28);
        day_of_week         = lut[(uxx)today.day_of_week()];
        xhint(today.year(), today.month(), today.day(), day_of_week);
    }

    // 演示：今天是今年第几天
    {
        xhint(demo);

        date<> today        = now;
        date<> the_date;
        xhint(today.year(), today.month(), today.day(), today.day_of_year(), today.day_index_of_year());

        // 第 1 天，索引 0
        the_date.year(2020).month(1).day(1);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.day_of_year(), the_date.day_index_of_year());

        // +31
        // 第 32 天，索引 31
        the_date.year(2020).month(2).day(1);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.day_of_year(), the_date.day_index_of_year());
        the_date.year(2019).month(2).day(1);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.day_of_year(), the_date.day_index_of_year());

        // +29
        // 第 61 天，索引 60
        the_date.year(2020).month(3).day(1);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.day_of_year(), the_date.day_index_of_year());

        // +28
        // 第 60 天，索引 59
        the_date.year(2019).month(3).day(1);
        xhint(the_date.year(), the_date.month(), the_date.day(), the_date.day_of_year(), the_date.day_index_of_year());
    }

    // 演示：日期的比较
    {
        xhint(demo);

        date<> a;
        date<> b;
        a.year(2020);
        b.year(2021);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.year(2020).month(2);
        b.year(2020).month(1);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.year(2020).month(1).day(20);
        b.year(2020).month(1).day(21);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.year(2020).month(1).day(21);
        b.year(2020).month(1).day(21);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);

        a.year(2020).month(2).day(21);
        b.year(2021).month(1).day(21);

        xhint(a == b, a != b, a > b, a >= b, a < b, a <= b);
    }
};