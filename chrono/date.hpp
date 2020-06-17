#ifndef xpack_chrono_date
#define xpack_chrono_date
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::chrono_date
        #include"chrono/private/date.hpp"
        #include"chrono/private/day.hpp"
        #include"chrono/private/lut.hpp"
        #include"chrono/day.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::chrono_date::origin{
        inline day_t date::operator - (date value) const {
            return day_t(this[0]) - day_t(value);
        }

        inline date date::operator - (day_t value) const {
            day_t a = this[0];
            day_t b = a - value;
            return b.operator date();
        }

        inline date date::operator + (day_t value) const {
            day_t a = this[0];
            day_t b = a + value;
            return b.operator date();
        }

        inline date date::operator -= (day_t value){
            this[0] = this[0] - value;
            return this[0];
        }

        inline date date::operator += (day_t value){
            this[0] = this[0] + value;
            return this[0];
        }

        inline day_of_week_t date::day_of_week() const {
            return day_of_week_t(
                day_t(this[0]) % 7
            );
        }

        inline uxx date::day_of_year() const {
            auto days = is_leap() ? inc::sum_leap : inc::sum_normal;
            return days[month() - 1] + day();
        }

        inline uxx date::day_index_of_year() const {
            return day_of_year() - 1;
        }
    }
#endif

namespace xuser::inc{
    using namespace ::mixc::chrono_date::origin;
}
