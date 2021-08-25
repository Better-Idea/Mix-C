#ifndef xpack_chrono_private_date_h
#define xpack_chrono_private_date_h
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_date::inc
#include"chrono/private/lookup_table.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"memop/cmp.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_day::origin{
    struct day;
}

namespace mixc::chrono_private_date{
    constexpr u08 month_leap  [] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    constexpr u08 month_normal[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    inline uxx days_of_month(bool is_leap, uxx month) {
        return (is_leap ? month_leap : month_normal)[month - 1];
    }
}

namespace mixc::chrono_private_date::origin{
    using day_t = mixc::chrono_private_day::origin::day;

    enum class day_of_week_t{
        sunday,     // 星期天
        monday,     // 星期一
        tuesday,    // 星期二
        wednesday,  // 星期三
        thursday,   // 星期四
        friday,     // 星期五
        saturday,   // 星期六
    };

    /**
     * @brief 
     * 日期类模板
     * @tparam final_t date<> 的最终类型
     * @tparam field_t 构成年月日字段的类型
     */
    template<class final_t, class field_t>
    xstruct(
        xtmpl(date   , final_t, field_t),
        xproc(m_day  , 5 , field_t),
        xproc(m_month, 4 , field_t),
        xproc(m_year , 23, field_t)  // 按照字节序用于比较
    )

        date(date const &) = default;

        /**
         * @brief 
         * 通过 chrono/now.hpp 初始化当前日期
         * @tparam now_t chrono/now.hpp 中的 now 符合该约束
         */
        template<class now_t>
        requires(now_t::im_now_t == true)
        date(now_t):
            date(now_t::date()){
        }

        /**
         * @brief
         * 通过与 date<> 相似的结构初始化该结构体
         * @tparam finalx_t value 的最终类型
         * @tparam fieldx_t value 年月日字段的类型
         * @param value 当前结构的类似物
         */
        template<class finalx_t , class fieldx_t>
        date(date<finalx_t, fieldx_t> const & value):
            date(value.year(), value.month(), value.day()){
        }

        /**
         * @brief
         * 通过年月日初始化该结构
         * @param year 当前年份
         * @param month 当前月份
         * @param day 几号
         */
        date(field_t year = 0, field_t month = 0, field_t day = 0) :
            m_day(day), m_month(month), m_year(year){
        }

        /**
         * @brief 
         * 当前年月日是否合法
         * @return true 合法
         * @return false 非法
         */
        bool is_valid() const {
            if (month() == 0 or month() > 12 or day() == 0 or year() == 0){
                return false;
            }

            auto max_day = days_of_month(is_leap(), month());
            return day() <= max_day;
        }

        /**
         * @brief 
         * 当前年份是否是闰年
         * @return true 是闰年
         * @return false 不是闰年
         */
        bool is_leap() const {
            return year() % 4 == 0 and (year() % 100 != 0 or year() % 400 == 0);
        }

        /**
         * @brief 
         * 得到明天对应的日期
         * @return final_t 
         * 返回一个新的对象，不改变当前对象的值
         */
        final_t tomorrow() const {
            auto max_day = days_of_month(is_leap(), month());

            if (day() + 1 <= max_day){
                return final_t{}.year(year()).month(month()).day(day() + 1);
            }
            if (month() + 1 <= 12){
                return final_t(year(), month() + 1, 1);
            }
            else{
                return final_t(year() + 1, 1, 1);
            }
        }

        /**
         * @brief 
         * 得到昨天对应的日期
         * @return final_t 
         * 返回一个新的对象，不改变当前对象的值
         */
        final_t yesterday() const {
            if (day() > 1){
                return final_t{}.year(year()).month(month()).day(day() - 1);
            }
            if (month() > 1){
                auto max_day = days_of_month(is_leap(), month() - 1);
                return final_t{}.year(year()).month(month() - 1).day(field_t(max_day));
            }
            else{
                return final_t{}.year(year() - 1).month(12).day(31);
            }
        }

        /**
         * @brief 
         * 今天星期几
         * @return day_of_week_t 
         */
        day_of_week_t day_of_week() const {
            return day_of_week_t(
                (day_t(this[0]) + 1) % 7
            );
        }

        /**
         * @brief 
         * 今天是今年第几天，从 1 开始计数
         * @return uxx 
         */
        uxx day_of_year() const {
            auto days = is_leap() ? inc::sum_leap : inc::sum_normal;
            return days[month() - 1] + the.day();
        }

        /**
         * @brief 
         * 今天是今年第几天，从 0 开始计数
         * @return uxx 
         */
        uxx day_index_of_year() const {
            return day_of_year() - 1;
        }

        /**
         * @brief 
         * 比较两个日期
         * @param value 参与比较的日期
         * @return ixx 
         * 当前日期 == value 返回 0
         * 当前日期 >  value 返回正数（不局限于 1）
         * 当前日期 <  value 返回负数（不局限于 -1）
         */
        ixx compare(the_t const & value) const {
            return inc::cmp_des(the, value); // 将结构转换成无符号整数序列，从高字往低字比较
        }

        /**
         * @brief 
         * 两个日期相差多少天
         * @param self 日期甲
         * @param value 日期乙
         * @return day_t 相差多少天（有符号数）
         */
        friend day_t operator - (the_t const & self, the_t const & value) {
            return day_t(self) - day_t(value);
        }

        /**
         * @brief 
         * 当前日期 value 天前对应的日期
         * @param self 日期甲
         * @param value 几天前
         * @return final_t 
         * 返回一个新的对象，不改变当前对象的值
         */
        friend final_t operator - (the_t const & self, day_t value) {
            day_t a = self;
            day_t b = a - value;
            return b;
        }

        /**
         * @brief 
         * 当前日期 value 天后对应的日期
         * @param self 日期甲
         * @param value 几天后
         * @return final_t 
         * 返回一个新的对象，不改变当前对象的值
         */
        friend final_t operator + (the_t const & self, day_t value) {
            day_t a = self;
            day_t b = a + value;
            return b;
        }

        /**
         * @brief 
         * 当前日期 value 天前对应的日期
         * @param self 日期甲
         * @param value 几天前
         * @return final_t 
         * 改变当前对象的值后，返回与当前对象值一样的新对象
         */
        friend final_t operator -= (the_t & self, day_t value){
            self = self - value;
            return self;
        }

        /**
         * @brief 
         * 当前日期 value 天后对应的日期
         * @param self 日期甲
         * @param value 几天前
         * @return final_t 
         * 改变当前对象的值后，返回与当前对象值一样的新对象
         */
        friend final_t operator += (the_t & self, day_t value){
            self = self + value;
            return self;
        }

        #define xa_args_list       the_t const & left, the_t const & right
        #define xa_invoke          left.compare(right)
        #define xa_is_friend
        #include"macro/xgen.cmp.hpp"

        xpubget_pubset(day)
        xpubget_pubset(month)
        xpubget_pubset(year)
    $
}

#endif
#define xusing_chrono_date      ::mixc::chrono_private_date::origin

xexport(mixc::chrono_private_date::origin::day_of_week_t)
