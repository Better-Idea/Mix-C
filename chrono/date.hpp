#ifdef xuser
#ifndef xusing_chrono_date
#include"chrono/private/date.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<class field_t = u32>
    xfinal_tmpl(xusing_chrono_date, date, field_t);
}

#undef  xusing_chrono_date
#endif
