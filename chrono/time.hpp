#ifndef xpack_chrono_time
#define xpack_chrono_time
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_time
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_time::origin{
    template<class final, class field_t = u32>
    xstruct(
        xtmpl(time_t, final, field_t)
    )
        time_t(u32 hour = 0, u32 minute = 0, u32 second = 0, u32 milisecond = 0) :
            pmilisecond(milisecond),
            psecond(second),
            pminute(minute),
            phour(hour){
        }

        template<class finalx, class f>
        time_t(time_t<finalx, f> const & value) : 
            time_t(
                value.phour,
                value.pminute,
                value.psecond,
                value.pmilisecond
            ){
        }

        template<class, class> friend struct time_t;

        #include"chrono/private/xgen.hpp"
        xgen(milisecond);
        xgen(second);
        xgen(minute);
        xgen(hour);
        #undef  xgen

        xpubgetx(is_valid, bool){
            return pminute <= 59 and psecond <= 59 and pmilisecond <= 999;
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
                phour       = value / (60 * 60 * 1000);
                value      %= (60 * 60 * 1000);
                pminute     = value / (60 * 1000);
                value      %= (60 * 1000);
                psecond     = value / (1000);
                value      %= (1000);
                pmilisecond = value;
            }

    protected:
        field_t pmilisecond     : 10;
        field_t psecond         : 6;
        field_t pminute         : 6;
        field_t phour           : sizeof(field_t) * 8 - 22;
    $

    struct time : time_t<time, u64>{
        using time_t<time, u64>::time_t;

        static time now();
    };
}
#endif

namespace xuser::inc{
    using namespace ::mixc::chrono_time::origin;
}
