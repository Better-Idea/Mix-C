#ifdef  xuser
#ifndef xusing_docker_stack
#include"docker/private/stack.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    using xusing_docker_stack::with_stack_policy_lock;
    using xusing_docker_stack::with_lwmutex;
    using xusing_docker_stack::lock_free;

    template<class item_t, xusing_docker_stack::is_attached_lock lock_t = xusing_docker_stack::lock_free>
    xfinal_tmpl(xusing_docker_stack, stack, item_t, lock_t);
}

#undef  xusing_docker_stack
#endif
