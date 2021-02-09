#ifndef xpack_chrono_private_time
#define xpack_chrono_private_time
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_time::inc
#include"memop/seqlize.hpp"
#include"memop/cmp.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_time::origin{
    constexpr uxx allow_leap_second = 60;

    template<class final, class field_t = u32>
    xstruct(
        xtmpl(time, final, field_t),
        xproc(pmilisecond, 10                      , field_t),  // 低位
        xproc(psecond    , 6                       , field_t),
        xproc(pminute    , 6                       , field_t),
        xproc(phour      , sizeof(field_t) * 8 - 22, field_t)   // 高位，按比较的顺序
    )
        template<class, class> friend struct time;

        time(time const &) = default;
        
        template<class now_t>
        requires(now_t::im_now_t == true)
        time(now_t):
            time(now_t::time()){
        }

        time(field_t hour = 0, field_t minute = 0, field_t second = 0, field_t milisecond = 0) :
            pmilisecond(milisecond),
            psecond(second),
            pminute(minute),
            phour(hour){
        }

        template<class finalx, class f>
        time(time<finalx, f> const & value) : 
            time(
                value.phour,
                value.pminute,
                value.psecond,
                value.pmilisecond
            ){
        }

        bool is_valid(uxx max_second = 59) const {
            return pminute <= 59 and psecond <= max_second and pmilisecond <= 999;
        }

        bool is_valid_24h_clock(uxx max_second = 59) const {
            return phour <= 23 and is_valid(max_second);
        }

        ixx compare(the_t const & value) const {
            return inc::cmp_des(the, value); // 将结构转换成无符号整数序列，从高字往低字比较
        }

        friend ixx operator- (the_t const & left, the_t const & right){
            return (ixx)(left.total_milisecond() - right.total_milisecond());
        }

        #define xa_args_list       the_t const & left, the_t const & right
        #define xa_invoke          compare(left, right)
        #define xa_is_friend
        #include"memop/cmp.gen.hpp"

        xpubget_pubset(milisecond)
        xpubget_pubset(second)
        xpubget_pubset(minute)
        xpubget_pubset(hour)

        xpubget_pubsetx(total_milisecond, u64)
            xr{
                auto ms = 
                    u64(pmilisecond) + 
                    u64(psecond) * (1000) + 
                    u64(pminute) * (60 * 1000) + 
                    u64(phour)   * (60 * 60 * 1000);
                return ms;
            }
            xw{
                phour       = (value / (60 * 60 * 1000));
                value      %= (60 * 60 * 1000);
                pminute     = (value / (60 * 1000));
                value      %= (60 * 1000);
                psecond     = (value / (1000));
                value      %= (1000);
                pmilisecond = (value);
            }
    $
}

#endif

#define xusing_chrono_time  ::mixc::chrono_private_time::origin
