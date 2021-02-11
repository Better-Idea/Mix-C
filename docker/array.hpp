#ifdef xuser
#ifndef xusing_docker_array
#include"docker/private/array.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    template<class item_t, uxx count = 0, uxx ... rest>
    xfinal_tmpl(xusing_docker_array, array, item_t, count, rest...);

    template<class item_t>
    struct array<item_t, 0> : 
        xusing_docker_array::array<array<item_t, 0>, item_t, 0>{
        using final_t = xusing_docker_array::array<array<item_t, 0>, item_t, 0>;
        using final_t::final_t;

        // 由于带 class final_t 的侵入式模板导致 C++ 编译器被绕晕，所以这里偷懒一下，在顶层定义 operator=
        // bad design
        final_t & operator= (decltype(nullptr)){ 
            thex.~final_t();
            return thex;
        }
    };

    template<class type, class ... args>
    struct array_view : array<type, 1 + sizeof...(args)>{
        array_view(type const & first, args const & ... list) : 
            array<type, 1 + sizeof...(args)>(first, list...) {
        }
    };
}

#undef  xusing_docker_array
#endif
