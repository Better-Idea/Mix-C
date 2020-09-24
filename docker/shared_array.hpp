#ifdef xuser
#ifndef xusing_docker_shared_array
#include"docker/private/shared_array.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    template<class item_t, uxx rank = 1, class attribute = void>
    xfinal_tmpl(xusing_docker_shared_array, shared_array, item_t, rank, attribute);
}

#undef  xusing_docker_shared_array
#endif
