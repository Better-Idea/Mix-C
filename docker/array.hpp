#ifdef xuser
#ifndef xusing_docker_array
#include"docker/private/array.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<class item_t, uxx count, uxx ... rest>
    xfinal_tmpl(xusing_docker_array, array, item_t, count, rest...);

    template<class type, class ... args>
    struct array_view : array<type, 1 + sizeof...(args)>{
        array_view(type const & first, args const & ... list) : 
            array<type, 1 + sizeof...(args)>(first, list...) {
        }
    };
}

#undef  xusing_docker_array
#endif
