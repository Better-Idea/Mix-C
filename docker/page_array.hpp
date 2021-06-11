#ifdef  xuser
#ifndef xusing_docker_page_array
#include"docker/private/page_array.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    using namespace xusing_docker_page_array::origin;

    template<class item_t, auto a0_v = pages_t(not_exist), auto a1_v = init_size_t(not_exist)>
    requires(xusing_docker_page_array::check_targ<a0_v, a1_v>)
    xfinal_tmpl(xusing_docker_page_array, page_array, item_t, a0_v, a1_v);
}

#undef  xusing_docker_page_array
#endif
