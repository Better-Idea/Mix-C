#ifndef xpack_chrono_private_time
#define xpack_chrono_private_time
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_time::inc
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"memop/seqlize.hpp"
#include"memop/cmp.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_time::origin{
    constexpr uxx allow_leap_second = 60;

    template<class final_t, class field_t = u32>
    xstruct(
        xtmpl(time, final_t, field_t),
        xproc(m_milisecond, 10                      , field_t),  // 低位
        xproc(m_second    , 6                       , field_t),
        xproc(m_minute    , 6                       , field_t),
        xproc(m_hour      , sizeof(field_t) * 8 - 22, field_t)   // 高位，按比较的顺序
    )
        template<class, class> friend struct time;

        time(time const &) = default;

        template<class now_t>
        requires(now_t::im_now_t == true)
        time(now_t):
            time(now_t::time()){
        }

        time(field_t hour = 0, field_t minute = 0, field_t second = 0, field_t milisecond = 0) :
            m_milisecond(milisecond),
            m_second(second),
            m_minute(minute),
            m_hour(hour){
        }

        template<class finalx_t , class f>
        time(time<finalx_t, f> const & value) : 
            time(
                value.m_hour,
                value.m_minute,
                value.m_second,
                value.m_milisecond
            ){
        }

        bool is_valid(uxx max_second = 59) const {
            return m_minute <= 59 and m_second <= max_second and m_milisecond <= 999;
        }

        bool is_valid_24h_clock(uxx max_second = 59) const {
            return m_hour <= 23 and is_valid(max_second);
        }

        ixx compare(the_t const & value) const {
            return inc::cmp_des(the, value); // 将结构转换成无符号整数序列，从高字往低字比较
        }

        friend ixx operator- (the_t const & left, the_t const & right){
            return (ixx)(left.total_milisecond() - right.total_milisecond());
        }

        #define xa_args_list       the_t const & left, the_t const & right
        #define xa_invoke          left.compare(right)
        #define xa_is_friend
        #include"macro/xgen.cmp.hpp"

        xpubget_pubset(milisecond)
        xpubget_pubset(second)
        xpubget_pubset(minute)
        xpubget_pubset(hour)

        xpubget_pubsetx(total_milisecond, u64)
            xr{
                auto ms = 
                    u64(m_milisecond) + 
                    u64(m_second) * (1000) + 
                    u64(m_minute) * (60 * 1000) + 
                    u64(m_hour)   * (60 * 60 * 1000);
                return ms;
            }
            xw{
                m_hour       = (value / (60 * 60 * 1000));
                value      %= (60 * 60 * 1000);
                m_minute     = (value / (60 * 1000));
                value      %= (60 * 1000);
                m_second     = (value / (1000));
                value      %= (1000);
                m_milisecond = (value);
            }
    $
}

#endif

#define xusing_chrono_time  ::mixc::chrono_private_time::origin
