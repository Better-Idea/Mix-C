#ifdef xuser
#ifndef xusing_docker_queue
#include"docker/private/queue.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser {
    template<class item_t, class barrier_t = void>
    xfinal_tmpl(xusing_docker_queue, queue, item_t, barrier_t);
}

#undef  xusing_docker_queue
#endif
