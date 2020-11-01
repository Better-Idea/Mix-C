#ifdef xuser
#ifndef xusing_docker_hashmap
#include"docker/private/hashmap.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<class key_t, class val_t = void>
    xfinal_tmpl(xusing_docker_hashmap, hashmap, key_t, val_t);
}

#undef  xusing_docker_hashmap
#endif
