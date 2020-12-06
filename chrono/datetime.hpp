#ifdef xuser
#ifndef xusing_chrono_datetime
#include"chrono/private/datetime.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    using ::mixc::chrono_private_time::origin::allow_leap_second;

    template<class field_t = u32>
    xfinal_tmpl(xusing_chrono_datetime, datetime, field_t);
}

#undef  xusing_chrono_datetime
#endif
