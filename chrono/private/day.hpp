#ifndef xpack_chrono_private_day
#define xpack_chrono_private_day
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::chrono_day
        #include"chrono/private/base_t.hpp"
        #include"chrono/private/date.hpp"
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::chrono_date::origin{
        struct date;
    }

    namespace mixc::chrono_day{
        using date = mixc::chrono_date::origin::date;
    }

    namespace mixc::chrono_day::origin{
        struct day : inc::base_t<day>{
            xgc_fields(
                xiam(day)
            );
        public:
            using inc::base_t<day>::base_t;

            day(date value);

            operator date();
        };
    }
#endif
