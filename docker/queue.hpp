#ifdef  xuser
#ifndef xusing_docker_queue
#include"docker/private/queue.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser {
    using xusing_docker_queue::with_queue_policy_lock;
    using xusing_docker_queue::with_lwmutex;
    using xusing_docker_queue::lock_free;

    template<class item_t, xusing_docker_queue::is_attached_lock lock_t = xusing_docker_queue::lock_free>
    xfinal_tmpl(xusing_docker_queue, queue, item_t, lock_t);
}

#undef  xusing_docker_queue
#endif
