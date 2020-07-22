#ifdef xuser
#ifndef xusing_docker_priority_queue
#include"docker/private/priority_queue.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc {
    template<class item_t>
    xfinal_tmpl(xusing_docker_name, priority_queue, item_t);
}

#undef  xusing_docker_priority_queue
#endif
