#ifdef xuser
#ifndef xusing_docker_queue
#include"docker/private/queue.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc {
    template<class item_t>
    xfinal_tmpl(xusing_docker_queue, queue, item_t);
}

#undef  xusing_docker_queue
#endif
