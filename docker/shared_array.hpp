#ifdef xuser
#ifndef xusing_docker_shared_array
#include"docker/private/shared_array.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    #ifndef xdocker_shared_array_disable_is_binary_aligned_alloc
        template<class item_t, uxx rank = 1, class attribute = void, bool is_binary_aligned_alloc = false>
        xfinal_tmpl(xusing_docker_shared_array, shared_array, item_t, rank, attribute, is_binary_aligned_alloc);
    #else
        #undef xdocker_shared_array_disable_is_binary_aligned_alloc
        template<class item_t, uxx rank = 1, class attribute = void>
        xfinal_tmpl(xusing_docker_shared_array, shared_array, item_t, rank, attribute);
    #endif
}

#undef  xusing_docker_shared_array
#endif
