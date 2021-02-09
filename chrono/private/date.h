#ifndef xpack_chrono_private_date_h
#define xpack_chrono_private_date_h
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_date::inc
#include"chrono/private/lut.hpp"
#include"memop/cmp.hpp"
#include"mixc.hpp"
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
        sunday,
        monday,
        tuesday,
        wednesday,
        thursday,
        friday,
        saturday,
    };

    template<class final, class field_t>
    xstruct(
        xtmpl(date  , final, field_t),
        xproc(pday  , 5 , field_t),
        xproc(pmonth, 4 , field_t),
        xproc(pyear , 23, field_t)  // 按照字节序用于比较
    )

        date(date const &) = default;

        template<class now_t>
        requires(now_t::im_now_t == true)
        date(now_t):
            date(now_t::date()){
        }

        template<class finalx, class fieldx>
        date(date<finalx, fieldx> const & object):
            date(object.year(), object.month(), object.day()){
        }

        date(field_t year = 0, field_t month = 0, field_t day = 0) :
            pday(day), pmonth(month), pyear(year){
        }

        bool is_valid() const {
            if (month() == 0 or month() > 12 or day() == 0 or year() == 0){
                return false;
            }

            auto max_day = days_of_month(is_leap(), month());
            return day() <= max_day;
        }

        bool is_leap() const {
            return year() % 4 == 0 and (year() % 100 != 0 or year() % 400 == 0);
        }

        final tomorrow() const {
            auto max_day = days_of_month(is_leap(), month());

            if (day() + 1 <= max_day){
                return final{}.year(year()).month(month()).day(day() + 1);
            }
            if (month() + 1 <= 12){
                return final(year(), month() + 1, 1);
            }
            else{
                return final(year() + 1, 1, 1);
            }
        }

        final yesterday() const {
            if (day() > 1){
                return final{}.year(year()).month(month()).day(day() - 1);
            }
            if (month() > 1){
                auto max_day = days_of_month(is_leap(), month() - 1);
                return final{}.year(year()).month(month() - 1).day(field_t(max_day));
            }
            else{
                return final{}.year(year() - 1).month(12).day(31);
            }
        }

        day_of_week_t day_of_week() const {
            return day_of_week_t(
                (day_t(this[0]) + 1) % 7
            );
        }

        uxx day_of_year() const {
            auto days = is_leap() ? inc::sum_leap : inc::sum_normal;
            return days[month() - 1] + the.day();
        }

        uxx day_index_of_year() const {
            return day_of_year() - 1;
        }

        ixx compare(the_t const & value) const {
            return inc::cmp_des(the, value); // 将结构转换成无符号整数序列，从高字往低字比较
        }

        friend day_t operator - (the_t const & self, the_t const & value) {
            return day_t(self) - day_t(value);
        }

        friend final operator - (the_t const & self, day_t value) {
            day_t a = self;
            day_t b = a - value;
            return b;
        }

        friend final operator + (the_t const & self, day_t value) {
            day_t a = self;
            day_t b = a + value;
            return b;
        }

        friend final operator -= (the_t & self, day_t value){
            self = self - value;
            return self;
        }

        friend final operator += (the_t & self, day_t value){
            self = self + value;
            return self;
        }

        #define xa_args_list       the_t const & left, the_t const & right
        #define xa_invoke          left.compare(right)
        #define xa_is_friend
        #include"memop/cmp.gen.hpp"

        xpubget_pubset(day)
        xpubget_pubset(month)
        xpubget_pubset(year)
    $
}

#endif
#define xusing_chrono_date      ::mixc::chrono_private_date::origin

xexport(mixc::chrono_private_date::origin::day_of_week_t)
