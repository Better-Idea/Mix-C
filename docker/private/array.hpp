#ifndef xpack_docker_array
#define xpack_docker_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_array
#include"docker/private/adapter.array_access.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_array{
    template<class type, uxx count>
    using items_t = type[count];

    template<class final, class type, uxx count = 0, uxx ... rest>
    xstruct(
        xtmpl(array_t, final, type, count, rest...),
        xprif(data, mutable items_t<typename array_t<final, type, rest...>::the_t, count>)
    )
        using item_t = typename array_t<final, type, rest...>::the_t;
    public:
        array_t() : data(){}

        template<class ... args>
        array_t(item_t const & first, args const & ... list) : 
            data { first, ((item_t)list)... } {}

        array_t(array_t const &) = default;

        template<class finalx>
        array_t(array_t<finalx, type, count, rest...> const & self) : 
            array_t((the_t &)(array_t<finalx, type, count, rest...> &)self) {
            static_assert(sizeof(self) == sizeof(the_t));
        }

        item_t & operator[] (uxx index) {
            return data[index];
        }

        item_t const & operator[] (uxx index) const {
            return data[index];
        }

        operator item_t *() {
            return data;
        }

        operator item_t const *() const {
            return data;
        }
        
        xpubgetx(length, uxx){
            return count;
        }
    $

    template<class final, class type>
    struct array_t<final, type>{
        using the_t = type;
    };

    template<class final, class type, uxx count, uxx ... rest>
    using array = inc::adapter_array_access<
        array_t<final, type, count, rest...>,
        type
    >;
}

#endif

#include"math/index_system.hpp"

#undef  xusing_docker_array
#define xusing_docker_array     ::mixc::docker_array
