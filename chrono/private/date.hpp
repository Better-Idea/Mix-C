#ifndef xpack_chrono_private_date
#define xpack_chrono_private_date
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::chrono_date
        #include"chrono/private/lut.hpp"
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
        #include"memop/cmp.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::chrono_day::origin{
        struct day;
    }
    
    namespace mixc::chrono_date{
        using day_t = mixc::chrono_day::origin::day;

        constexpr u08 month_leap  [] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        constexpr u08 month_normal[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        inline uxx days_of_month(bool is_leap, uxx month) {
            return (is_leap ? month_leap : month_normal)[month];
        }
    }
    
    namespace mixc::chrono_date::origin{
        enum class day_of_week_t{
            sunday,
            monday,
            tuesday,
            wednesday,
            thursday,
            friday,
            saturday,
        };

        template<class final> struct date_t;

        struct date{
            xgc_fields(
                xiam(date)
            );
            using final = date;
        public:
            day_t           operator -  (date  value) const;
            date            operator -  (day_t value) const;
            date            operator +  (day_t value) const;
            date            operator -= (day_t value);
            date            operator += (day_t value);
            day_of_week_t   day_of_week() const;
            uxx             day_of_year() const;
            uxx             day_index_of_year() const;

            date(u32 year = 0, u32 month = 0, u32 day = 0) :
                pyear(year), pmonth(month), pday(day){
            }

            #include"chrono/private/xgen.hpp"
            xgen(day);
            xgen(month);
            xgen(year);
            #undef  xgen

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

            date tomorrow() const {
                auto max_day = days_of_month(is_leap(), month());

                if (day() + 1 <= max_day){
                    return date(year(), month(), day() + 1);
                }
                if (month() + 1 <= 12){
                    return date(year(), month() + 1, 1);
                }
                else{
                    return date(year() + 1, 1, 1);
                }
            }

            date yesterday() const {
                if (day() > 1){
                    return date(year(), month(), day() - 1);
                }
                if (month() > 1){
                    auto m       = month() - 1;
                    auto max_day = days_of_month(is_leap(), m);
                    return date(year(), m, max_day);
                }
                else{
                    return date(year() - 1, 12, 31);
                }
            }

            template<class finalx>
            operator date_t<finalx> & (){
                return *(date_t<finalx> *)this;
            }

            static date now();
        protected:
            u32 pday            : 5;
            u32 pmonth          : 4;
            u32 pyear           : 23;
        };

        template<class finalx>
        struct date_t : date{
            using date::date;
            using final = finalx;

            #include"chrono/private/xgen.hpp"
            xgen(day);
            xgen(month);
            xgen(year);
            #undef  xgen
        };
    }

    xcmpop(mixc::chrono_date::origin::date);
#endif
