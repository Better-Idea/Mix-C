#ifndef xpack_chrono_private_datetime
#define xpack_chrono_private_datetime
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_datetime::inc
#include"chrono/private/date.hpp"
#include"chrono/private/time.hpp"
#include"chrono/private/day.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"memop/cmp.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_datetime::inc{
    using mixc::chrono_private_date::origin::date;
    using mixc::chrono_private_time::origin::time;
    using mixc::chrono_private_day ::origin::day;
}

namespace mixc::chrono_private_datetime::origin{
    template<class final_t, class field_date = u32, class field_time = field_date>
    xstruct(
        xtmpl(datetime, final_t, field_date, field_time),
        xpubb(inc::time<final_t, field_time>),    // 低位
        xpubb(inc::date<final_t, field_date>)     // 高位 先比较
    )
    private:
        using the_time  = inc::time<final_t, field_time>;
        using the_date  = inc::date<final_t, field_date>;
        using the_time::is_valid_24h_clock;
        using the_time::total_milisecond;
        using the_time::compare;
        using the_date::compare;
    public:
        datetime(the_t const &) = default;

        template<class now_t>
        requires(now_t::im_now_t == true)
        datetime(now_t):
            datetime(now_t::datetime()){
        }

        template<class finalx_t , class field_datex, class field_timex>
        datetime(datetime<finalx_t, field_datex, field_timex> const & object):
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
        ) : the_time(hour, minute, second, milisecond),
            the_date(year, month, day){
        }

        template<class finala_t, class field_datex_t, class finalb_t, class field_timex_t>
        datetime(
            inc::date<finala_t, field_datex_t> const & date, 
            inc::time<finalb_t, field_timex_t> const & time): 
            the_time(time), the_date(date){
        }

        template<class finala_t, class field_datex_t>
        datetime(inc::date<finala_t, field_datex_t> date): 
            the_time(), the_date(date){
        }

        template<class finala_t, class field_timex_t>
        datetime(inc::time<finala_t, field_timex_t> const & time): 
            the_time(time), the_date(){
        }

        bool is_valid(uxx max_second = 59) const {
            return 
                inc::date<final_t, field_date>::is_valid() and 
                inc::time<final_t, field_time>::is_valid_24h_clock(max_second);
        }

        ixx compare(the_t const & value) const {
            return inc::cmp_des(the, value); // 将结构转换成无符号整数序列，从高字往低字比较
        }

        friend ixx operator- (the_t const & left, the_t const & right) {
            return (ixx)(left.total_milisecond() - right.total_milisecond());
        }

        friend final_t operator- (the_t const & left, inc::day right) {
            final_t r         = (final_t &)(the_t &)left;
            (the_date &)r  -= right;
            return r;
        }

        friend final_t operator-= (the_t & left, inc::day right) {
            left = left - right;
            return left;
        }

        friend final_t operator+ (the_t const & left, inc::day right) {
            final_t r         = (final_t &)(the_t &)left;
            (the_date &)r  += right;
            return r;
        }

        friend final_t operator+= (the_t & left, inc::day right) {
            left = left + right;
            return left;
        }

        #define xa_args_list       the_t const & left, the_t const & right
        #define xa_invoke          left.compare(right)
        #define xa_is_friend
        #include"macro/xgen.cmp.hpp"

        xpubget_pubsetx(total_milisecond, u64)
            xr{
                constexpr u64 one_day = 24 * 3600 * 1000;
                u64 part_a  = inc::time<final_t, field_time>::total_milisecond();
                u64 part_b  = one_day * inc::day(the);
                u64 ms      = part_a + part_b;
                return ms;
            }
            xw{
                constexpr u64 one_day = 24 * 3600 * 1000;
                inc::time<final_t, field_time>::total_milisecond(value % one_day);
                value      /= one_day;
                the         = inc::day(value);
            }
    $
}

#endif

#define xusing_chrono_datetime ::mixc::chrono_private_datetime::origin
