#ifndef xpack_chrono_datetime
#define xpack_chrono_datetime
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::chrono_datetime
        #include"chrono/date.hpp"
        #include"chrono/time.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::chrono_datetime{
        using namespace inc;
    }

    namespace mixc::chrono_datetime::origin{
        template<class final>
        struct datetime_t : date_t<final>, time_t<final>{
            using date_t<final>::date_t;

            datetime_t(
                u32 year, 
                u32 month, 
                u32 day, 
                u32 hour, 
                u32 minute, 
                u32 second, 
                u32 milisecond
            ) : date_t<final>(year, month, day), 
                time_t<final>(hour, minute, second, milisecond){
            }

            bool is_valid() const {
                return date_t<final>::is_valid() and time_t<final>::is_valid() and time_t<final>::hour() <= 23;
            }
        };

        struct datetime : datetime_t<datetime>{
            using datetime_t<datetime>::datetime_t;

            static datetime now();
        };
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::chrono_datetime::origin;
}
