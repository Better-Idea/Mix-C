#ifdef  xuser
#ifndef xusing_docker_btree
#include"docker/private/btree.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<class item_t>
    xfinal_tmpl(xusing_docker_btree, btree, item_t);
}

#undef  xusing_docker_btree
#endif
