#define xuser mixc::powerful_cat
#include"chrono/day.hpp"
#include"chrono/date.hpp"
#include"mixc.hpp"

namespace xuser{
    void demo(){
        uxx demo = 0;

        // 演示：
        {
            xhint(demo);
            demo           += 1;
            
            // 创建日期
            date<> the_date{ 2020/*year*/, 12/*month*/, 5/*day*/ };

            // 从公元元年开始开始计数，一共经过的天数
            day d           = the_date;

            // 转换成数值
            uxx total_days  = d;

            xhint(total_days);

            // 转换成日期
            date<> new_date = d;

            xhint(new_date.year(), new_date.month(), new_date.day());
        }
    }
}

int main(){
    xuser::demo();
    return 0;
}