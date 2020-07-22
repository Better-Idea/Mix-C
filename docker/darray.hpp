#ifdef xuser
#ifndef xusing_docker_darray
#include"docker/private/darray.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    template<class item_t, uxx rank = 1, class attribute = void>
    xfinal_tmpl(xusing_docker_darray, darray, item_t, rank, attribute);
}

#undef  xusing_docker_darray
#endif
