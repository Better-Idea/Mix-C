#ifndef xpack_docker_tuple
#define xpack_docker_tuple
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_tuple::inc
#include"define/nullref.hpp"
#include"docker/array.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_origin_array.hpp"
#include"meta/item_origin_of.hpp"
#include"meta/remove_ptr.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_tuple{
    template<class a0_t>
    inline auto meta(){
        if constexpr (inc::is_origin_array<a0_t>){
            using item_t = inc::item_origin_of<a0_t>;
            return (inc::array<item_t, sizeof(a0_t) / sizeof(item_t)> *)nullptr;
        }
        else{
            return (a0_t *)nullptr;
        }
    }

    template<class a0_t>
    using item_t = inc::remove_ptr<
        decltype(meta<a0_t>())
    >;
}

namespace mixc::docker_tuple::origin{
    template<class a0_t = decltype(nullptr), class ... args_t>
    xstruct(
        xtmpl(tuple, a0_t, args_t...),
        xprob(tuple<args_t...>),
        xprif(value, item_t<a0_t>)
    )
        using base = tuple<args_t...>;

        template<class ...>
        constexpr tuple(){}

        constexpr tuple(a0_t const & first, args_t const & ... result) : 
            value(*(item_t<a0_t> *)& first), base(result...){
        }

        template<uxx index>
        auto & get() const {
            if constexpr (index == 0){
                return value;
            }
            else{
                return base::template get<index - 1>();
            }
        }

        template<uxx index, class val_t>
        auto & set(val_t const & value){
            get<index>() = value;
            return this[0];
        }

        static constexpr uxx length(){
            return 1 + sizeof...(args_t);
        }
    $

    template<>
    xstruct(
        xspec(tuple, decltype(nullptr))
    )
        constexpr tuple(){}

        template<uxx index>
        auto & get() const {
            return inc::nullref;
        }
    $
}

#endif

xexport_space(mixc::docker_tuple::origin)