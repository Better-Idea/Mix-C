#ifdef xuser
#ifndef xusing_utils_bit_indicator
#include"utils/private/bits_indicator.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<uxx bits_v = 0>
    xfinal_tmpl(xusing_utils_bit_indicator, bits_indicator, bits_v);
}

#undef xusing_utils_bit_indicator
#endif
