#ifdef xuser
#ifndef xusing_docker_stack
#include"docker/private/stack.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    using xusing_docker_stack::with_stack_lock;

    template<class item_t, class lock_t = void>
    xfinal_tmpl(xusing_docker_stack, stack, item_t, lock_t);
}

#undef  xusing_docker_stack
#endif
