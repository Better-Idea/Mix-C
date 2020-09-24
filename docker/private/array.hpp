#ifndef xpack_docker_array
#define xpack_docker_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_array
#include"docker/private/adapter.foreach.array.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_array{
    template<class type, uxx count>
    using items_t = type[count];

    template<class type, uxx count = 0, uxx ... rest>
    xstruct(
        xtmpl(array_t, type, count, rest...),
        xprif(data, mutable items_t<typename array_t<type, rest...>::the_t, count>)
    )
        using item_t = typename array_t<type, rest...>::the_t;
    public:
        array_t() : data(){}

        template<class ... args>
        array_t(item_t const & first, args const & ... list) : 
            data { first, ((item_t)list)... } {}

        item_t & operator[] (uxx index) const {
            return data[index];
        }

        template<class number_t>
        item_t & operator[] (number_t const & index) const {
            return data[(uxx)(number_t &)index];
        }

        constexpr uxx length() const {
            return count;
        }

        operator item_t *() const {
            return data;
        }
    };

    template<class type>
    struct array_t<type>{
        using the_t = type;
    };

    template<class final, class type, uxx count, uxx ... rest>
    using array = inc::adapter_foreach_array<
        final, 
        array_t<type, count, rest...>,
        type
    >;
}

#endif

#include"math/index_system.hpp"

#undef  xusing_docker_array
#define xusing_docker_array     ::mixc::docker_array
