#ifndef xpack_chrono_private_day_h
#define xpack_chrono_private_day_h
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_day::inc
#include"chrono/private/lookup_table.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"meta/has_constructor.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_date::origin{
    template<class final_t, class field_t> struct date;
}

namespace mixc::chrono_private_day::inc{
    using mixc::chrono_private_date::origin::date;
}

namespace mixc::chrono_private_day::origin{
    /**
     * @brief 
     * 天数，可以为负数，两个 date<> 的差 依赖此性质
     */
    xstruct(
        xname(day),
        xprif(m_value, ixx)
    )
        day(ixx value = 0) : 
            m_value(value){}

        /**
         * @brief
         * 将日期转换成天数
         * @tparam finalx_t value 最终类型
         * @tparam fieldx_t value 存放年月日变量对应的类型
         * @param value 日期
         */
        template<class finalx_t , class fieldx_t>
        day(inc::date<finalx_t, fieldx_t> value){
            auto y = value.year() - 1;
            auto m = value.month() - 1;
            auto d = value.day();
            auto a = y % 400;
            auto b = a % 100;
            auto c = value.is_leap() ? inc::sum_leap[m] : inc::sum_normal[m];
            m_value = y * 365 + y / 400 * 97 + a / 100 * 24 + b / 4 + c + d - 1;
        }

        operator ixx & (){
            return m_value;
        }

        operator ixx const & () const {
            return m_value;
        }

        day operator = (ixx value){
            m_value = value;
            return the;
        }

        /**
         * @brief 
         * 将天数转回日期
         * @tparam final_date_t 存在通过 年月日 构造的类型
         */
        template<class final_date_t>
        requires(inc::has_constructor<final_date_t, void(u32, u32, u32)>)
        operator final_date_t() const {
            constexpr u32 a = 400 * 365 + 97; // 完整的 400 年共有的天数
            constexpr u32 b = 100 * 365 + 24; // 完整的 100 年共有的天数
            constexpr u32 c = 4   * 365 + 1;  // 完整的 4   年共有的天数

            auto v0 = m_value + 1;
            auto p0 = v0 / a * 400;
            auto p1 = v0 % a;
            auto p2 = p1 / b * 100;
            auto p3 = p1 % b;
            auto p4 = p3 / c * 4;
            auto p5 = p3 % c;

            if (p5 == 0){
                return final_date_t(p0 + p2 + p4, 12, 30 + (p2 != 400));
            }

            auto p6 = p5 / 365;
            auto p7 = p5 % 365;

            if (p7 == 0){
                return final_date_t(p0 + p2 + p4 + p6, 12, 30 + (p6 != 4));
            }

            auto da = final_date_t(); da.year(p0 + p2 + p4 + p6 + 1);
            auto lu = da.is_leap() ? inc::sum_leap : inc::sum_normal;
            auto p8 = p7 / 31;
            auto p9 = 
                p7 > lu[p8 + 1] ? p8 + 1 :
                p7 > lu[p8] ? p8 : p8 - 1;
            auto pa = p7 - lu[p9];
            da.month(p9 + 1);
            da.day(pa);
            return da;
        }
    $
}

#endif

xexport(mixc::chrono_private_day::origin::day)
