#ifdef xuser
#ifndef xusing_utils_bit_indicator
#include"utils/private/bit_indicator.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<uxx bits = 0>
    xfinal_tmpl(xusing_utils_bit_indicator, bit_indicator, bits);
}

#undef xusing_utils_bit_indicator
#endif
