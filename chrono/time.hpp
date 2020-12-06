#ifdef xuser
#ifndef xusing_chrono_time
#include"chrono/private/time.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    using ::mixc::chrono_private_time::origin::allow_leap_second;

    template<class field_t = u32>
    xfinal_tmpl(xusing_chrono_time, time, field_t);
}

#undef  xusing_chrono_time
#endif
