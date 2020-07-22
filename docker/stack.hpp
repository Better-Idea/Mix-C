#ifdef xuser
#ifndef xusing_docker_stack
#include"docker/private/stack.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    template<class item_t>
    xfinal_tmpl(xusing_docker_stack, stack);
}

#undef  xusing_docker_stack
#endif
