#ifndef xpack_chrono_day
#define xpack_chrono_day
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::chrono_day
        #include"chrono/private/day.hpp"
        #include"chrono/private/lut.hpp"
        #include"chrono/date.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::chrono_day::origin{
        inline day::day(date value){
            auto y = value.year() - 1;
            auto m = value.month() - 1;
            auto d = value.day();
            auto a = y % 400;
            auto b = a % 100;
            auto c = value.is_leap() ? inc::sum_leap[m] : inc::sum_normal[m];
            pvalue = y * 365 + y / 400 * 97 + a / 100 * 24 + b / 4 + c + d - 1;
        }

        inline day::operator date(){
            constexpr uxx a = 400 * 365 + 97; // 完整的 400 年共有的天数
            constexpr uxx b = 100 * 365 + 24; // 完整的 100 年共有的天数
            constexpr uxx c = 4 * 365 + 1;    // 完整的 4   年共有的天数

            auto v0 = pvalue + 1;
            auto p0 = v0 / a * 400;
            auto p1 = v0 % a;
            auto p2 = p1 / b * 100;
            auto p3 = p1 % b;
            auto p4 = p3 / c * 4;
            auto p5 = p3 % c;

            if (p5 == 0){
                return date(p0 + p2 + p4, 12, 30 + (p2 != 400));
            }

            auto p6 = p5 / 365;
            auto p7 = p5 % 365;

            if (p7 == 0){
                return date(p0 + p2 + p4 + p6, 12, 30 + (p6 != 4));
            }

            auto da = date().year(p0 + p2 + p4 + p6 + 1);
            auto lu = da.is_leap() ? inc::sum_leap : inc::sum_normal;
            auto p8 = p7 / 31;
            auto p9 = 
                p7 > lu[p8 + 1] ? p8 + 1 :
                p7 > lu[p8] ? p8 : p8 - 1;
            auto pa = p7 - lu[p9];
            return da.month(p9 + 1).day(pa);
        }
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::chrono_day::origin;
}
