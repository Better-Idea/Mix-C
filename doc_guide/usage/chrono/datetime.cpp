#define xuser mixc::powerful_cat
#include"chrono/day.hpp"
#include"chrono/datetime.hpp"
#include"chrono/time.hpp"
#include"chrono/now.hpp"
#include"mixc.hpp"

namespace xuser{
    void demo(){
        uxx demo = 0;

        auto show = [](auto d){
            xhint(
                d.year(),
                d.month(), 
                d.day(), 
                d.hour(), 
                d.minute(), 
                d.second(), 
                d.milisecond()
            );
        };

        // 演示：获取当前日期时间
        {
            xhint(demo);
            demo               += 1;

            // 获取当前日期时间
            datetime<> date_now = now;

            show(date_now);
        }

        // 演示：推荐的初始化日期方法
        {
            xhint(demo);
            demo               += 1;

            datetime<> d;

            // 通过链式调用设置日期属性
            // 创建日期
            d.year(2020).month(12).day(6).hour(10).minute(28).second(59).milisecond(998);

            show(d);
        }

        // 演示：判断日期是否合法
        {
            xhint(demo);
            demo               += 1;

            datetime<> d;

            // 通过链式调用设置日期属性
            // 创建日期
            d.year(2020).month(12).day(6);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // year 必须大于 0 
            // 该日期不合法
            d.year(zero).month(12).day(6);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // month 必须大于 0 
            // 该日期不合法
            d.year(2020).month(zero).day(6);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // month 必须小于等于 12 
            // 该日期不合法
            d.year(2020).month(13).day(6);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // day 必须大于 0 
            // 该日期不合法
            d.year(2020).month(12).day(zero);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // day 必须小于等于当月最大的天数
            // day 溢出
            // 该日期不合法
            d.year(2020).month(12).day(32);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // 因为 2020 年是闰年，所以 2 月有 29 天
            // 该日期合法
            d.year(2020).month(2).day(29);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // 因为 2019 年是平年，所以 2 月只有 28 天
            // 该日期不合法
            d.year(2019).month(2).day(29);
            xhint(d.is_valid(), d.year(), d.month(), d.day());

            // 设置时间部分 ================================================================
            d.year(2021).month(1).day(1);

            // 对于 datetime，time 和 date 是一个整体
            // 所以 hour 不能大于 23
            d.hour(24).minute(zero).second(zero).milisecond(zero);
            xhint(d.is_valid(), d.hour(), d.minute(), d.second(), d.milisecond());

            // minute 不能大于 59
            d.hour(zero).minute(59).second(zero).milisecond(zero);
            xhint(d.is_valid(), d.hour(), d.minute(), d.second(), d.milisecond());
            d.minute(60);
            xhint(d.is_valid(), d.hour(), d.minute(), d.second(), d.milisecond());

            // second 不能大于 59
            d.minute(59).second(59);
            xhint(d.is_valid(), d.hour(), d.minute(), d.second(), d.milisecond());
            d.second(60);
            xhint(d.is_valid(), d.hour(), d.minute(), d.second(), d.milisecond());

            // 当允许润秒时 second 最大可以为 60
            xhint(d.is_valid(allow_leap_second), d.hour(), d.minute(), d.second(), d.milisecond());

            // 但不能超过 60
            d.second(61);
            xhint(d.is_valid(allow_leap_second), d.hour(), d.minute(), d.second(), d.milisecond());

            // milisecond 不能大于 999
            d.second(zero).milisecond(999);
            xhint(d.is_valid(), d.hour(), d.minute(), d.second(), d.milisecond());
            d.milisecond(1000);
            xhint(d.is_valid(), d.hour(), d.minute(), d.second(), d.milisecond());
        }

        // 演示：判断当前年份是否是闰年
        {
            xhint(demo);
            demo               += 1;

            datetime<> d;

            // 2003 不能被 4 整除，所以不是闰年
            xhint(d.year(2003).is_leap());

            // 1900 年由于能被 100 整除所以不是闰年
            xhint(d.year(1900).is_leap());

            // 2000 年是虽然能被 100 整除，由于也能被 400 整除，所以是闰年
            xhint(d.year(2000).is_leap());

            // 2004 年是闰年
            xhint(d.year(2004).is_leap());
        }

        // 演示：计算明天对应的日期
        {
            xhint(demo);
            demo               += 1;

            datetime<> d;
            d.year(2020).month(2).day(28);

            // 明天是 2020/2/29
            d = d.tomorrow();
            xhint(d.year(), d.month(), d.day());

            // 明天是 2020/3/1
            d = d.tomorrow();
            xhint(d.year(), d.month(), d.day());

            // 明天是 2020/12/31
            d.year(2020).month(12).day(30);
            d = d.tomorrow();
            xhint(d.year(), d.month(), d.day());

            // 明天是 2021/1/1
            d = d.tomorrow();
            xhint(d.year(), d.month(), d.day());

            // 注意：
            // year 能表示的范围为[0, 2^23)，所以一直迭代计算 tomorrow 会溢出，如果必要需要做边界处理
        }

        // 演示：计算昨天对应的日期
        {

            xhint(demo);
            demo               += 1;

            datetime<> d;
            d.year(2020).month(3).day(2);

            // 昨天是 2020/3/1
            d = d.yesterday();
            xhint(d.year(), d.month(), d.day());

            // 昨天是 2020/2/29
            d = d.yesterday();
            xhint(d.year(), d.month(), d.day());

            // 昨天是 2020/1/1
            d.year(2020).month(1).day(2);
            d = d.yesterday();
            xhint(d.year(), d.month(), d.day());

            // 昨天是 2019/12/31
            d = d.yesterday();
            xhint(d.year(), d.month(), d.day());

            // 注意：
            // year 能表示的范围为[0, 2^23)，所以一直迭代计算 yesterday 会溢出，如果必要需要做边界处理
        }

        // 演示：日期减法
        {

            xhint(demo);
            demo               += 1;

            datetime<> d;
            datetime<> old_date;

            // 2 天前是 2020/2/29
            d.year(2020).month(3).day(2);
            d = d - day{2};
            xhint(d.year(), d.month(), d.day());

            // 1 年前是 2019/2/28
            d = d - day{366}; /*2020 年全年 366 天*/
            xhint(d.year(), d.month(), d.day());

            old_date.year(2020).month(1).day(1);
            d.year(2021).month(1).day(1);

            // 注意：
            // datetime 减法包括 time 部分的运算，所以最终结果的单位是 ms 毫秒
            // 2020 年一共有 31‘622’400‘000ms
            i64 diff = d - old_date;
            xhint(diff);
        }

        // 演示：日期加法
        {
            xhint(demo);
            demo               += 1;

            datetime<> d;
            datetime<> old_date;

            // 27 天后是 2020/2/28
            d.year(2020).month(2).day(1);
            d = d + day{27};
            xhint(d.year(), d.month(), d.day());

            // 1 年后是 2021/2/28
            d = d + day{366}; /*2020 年全年 366 天*/
            xhint(d.year(), d.month(), d.day());

            // 注意该日期是元年第一天
            // 也就是所谓的第 0 日
            old_date.year(1).month(1).day(1);
            d.year(2021).month(1).day(1);

            // 日期的相加就和指针的乘法一样并没有什么太大的意义
            // 我们并没有提供显式的日期之间的加法，而是日期可以转成 day 类型
            // 而 datetime 实现的是日期加减天数的运算重载
            d = d + old_date/*隐式转成 day 类型*/;

            // 由于加的是第 0 日，所以日期不变
            xhint(d.year(), d.month(), d.day());
        }

        // 演示：今天是星期几
        {
            xhint(demo);
            demo               += 1;

            datetime<> today    = now;
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
            demo               += 1;

            datetime<> today    = now;
            datetime<> d;
            xhint(today.year(), today.month(), today.day(), today.day_of_year(), today.day_index_of_year());

            // 第 1 天，索引 0
            d.year(2020).month(1).day(1);
            xhint(d.year(), d.month(), d.day(), d.day_of_year(), d.day_index_of_year());

            // +31
            // 第 32 天，索引 31
            d.year(2020).month(2).day(1);
            xhint(d.year(), d.month(), d.day(), d.day_of_year(), d.day_index_of_year());
            d.year(2019).month(2).day(1);
            xhint(d.year(), d.month(), d.day(), d.day_of_year(), d.day_index_of_year());

            // +29
            // 第 61 天，索引 60
            d.year(2020).month(3).day(1);
            xhint(d.year(), d.month(), d.day(), d.day_of_year(), d.day_index_of_year());

            // +28
            // 第 60 天，索引 59
            d.year(2019).month(3).day(1);
            xhint(d.year(), d.month(), d.day(), d.day_of_year(), d.day_index_of_year());
        }

        // 演示：计算总毫秒数
        {
            xhint(demo);
            demo           += 1;

            // 创建时间
            datetime<>  d;
            u64         total_ms;
            d.year(2020).month(12).day(6).hour(1).minute(2).second(3).milisecond(4);

            total_ms        = d.total_milisecond();
            xhint(total_ms);
        }
    }
}

int main(){
    xuser::demo();
    return 0;
}