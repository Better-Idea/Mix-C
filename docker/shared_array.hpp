#ifdef  xuser
#ifndef xusing_docker_shared_array
#include"docker/private/shared_array.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    using namespace xusing_docker_shared_array::origin;

    #ifndef xdocker_shared_array_disable_is_binary_aligned_alloc
        template<class item_t, uxx rank_v = 1, class attribute_t = void, bool is_binary_aligned_alloc_v = false>
        xfinal_tmpl(xusing_docker_shared_array, shared_array, item_t, rank_v, attribute_t, is_binary_aligned_alloc_v);
    #else
        #undef xdocker_shared_array_disable_is_binary_aligned_alloc
        template<class item_t, uxx rank_v = 1, class attribute_t = void>
        xfinal_tmpl(xusing_docker_shared_array, shared_array, item_t, rank_v, attribute_t);
    #endif
}

#undef  xusing_docker_shared_array
#endif
