#ifdef xuser
#ifndef xusing_chrono_time
#include"chrono/private/time.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<class field_t = u32>
    xfinal_tmpl(xusing_chrono_time, time, field_t);
}

#undef  xusing_chrono_time
#endif
