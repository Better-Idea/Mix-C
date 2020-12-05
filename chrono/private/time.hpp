#ifndef xpack_chrono_private_time
#define xpack_chrono_private_time
#include"mixc.hpp"

namespace mixc::chrono_private_time::origin{
    template<class final, class field_t = u32>
    xstruct(
        xtmpl(time, final, field_t),
        xproc(pmilisecond, 10                      , field_t),
        xproc(psecond    , 6                       , field_t),
        xproc(pminute    , 6                       , field_t),
        xproc(phour      , sizeof(field_t) * 8 - 22, field_t)
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

        xpubget_pubset(milisecond)
        xpubget_pubset(second)
        xpubget_pubset(minute)
        xpubget_pubset(hour)

        xpubgetx(is_valid, bool){
            return pminute <= 59 and psecond <= 59 and pmilisecond <= 999;
        }

        xpubgetx(is_valid_allow_leap_second, bool){
            return pminute <= 60 and psecond <= 59 and pmilisecond <= 999;
        }

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
