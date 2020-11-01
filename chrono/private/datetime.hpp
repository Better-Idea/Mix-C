#ifndef xpack_chrono_private_datetime
#define xpack_chrono_private_datetime
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_datetime::inc
#include"chrono/private/date.hpp"
#include"chrono/private/time.hpp"
#include"chrono/private/day.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_datetime::inc{
    using mixc::chrono_private_date::origin::date;
    using mixc::chrono_private_time::origin::time;
    using mixc::chrono_private_day ::origin::day;
}

namespace mixc::chrono_private_datetime::origin{
    template<class final, class field_date = u32, class field_time = field_date>
    xstruct(
        xtmpl(datetime, final, field_date, field_time),
        xpubb(inc::date<final, field_date>),
        xpubb(inc::time<final, field_time>)
    )
        datetime(datetime const &) = default;

        template<class now_t>
        requires(now_t::im_now_t == true)
        datetime(now_t):
            datetime(now_t::datetime()){
        }

        template<class finalx, class field_datex, class field_timex>
        datetime(datetime<finalx, field_datex, field_timex> const & object):
            datetime(
                object.year(),
                object.month(),
                object.day(),
                object.hour(),
                object.minute(),
                object.second(),
                object.milisecond()
            ){
        }

        datetime(
            field_date year        = 0, 
            field_date month       = 0, 
            field_date day         = 0, 
            field_time hour        = 0, 
            field_time minute      = 0, 
            field_time second      = 0, 
            field_time milisecond  = 0
        ) : inc::date<final, field_date>(year, month, day), 
            inc::time<final, field_time>(hour, minute, second, milisecond){
        }

        template<class finala, class field_datex, class finalb, class field_timex>
        datetime(inc::date<finala, field_datex> date, inc::time<finalb, field_timex> time): 
            inc::date<final, field_date>(date), 
            inc::time<final, field_time>(time){
        }

        bool is_valid() const {
            return 
                inc::date<final, field_date>::is_valid() and 
                inc::time<final, field_time>::is_valid() and 
                inc::time<final, field_time>::hour() <= 23;
        }

        xpubget_pubsetx(total_milisecond, u64)
            xr{
                constexpr u64 one_day = 24 * 3600 * 1000;
                u64 part_a  = inc::time<final, field_time>::total_milisecond();
                u64 part_b  = one_day * inc::day(the);
                u64 ms      = part_a + part_b;
                return ms;
            }
            xw{
                constexpr u64 one_day = 24 * 3600 * 1000;
                inc::time<final, field_time>::total_milisecond(value % one_day);
                value      /= one_day;
                the         = inc::day(value);
            }
    $
}

#endif

#define xusing_chrono_datetime ::mixc::chrono_private_datetime::origin
