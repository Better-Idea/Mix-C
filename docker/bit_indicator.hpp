#ifdef xuser
#ifndef xusing_docker_bit_indicator
#include"docker/private/bit_indicator.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<uxx bits = 0>
    xfinal_tmpl(xusing_docker_bit_indicator, bit_indicator, bits);
}

#undef xusing_docker_bit_indicator
#endif
