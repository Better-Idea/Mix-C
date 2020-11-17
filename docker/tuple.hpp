#ifndef xpack_docker_tuple
#define xpack_docker_tuple
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_tuple::inc
#include"define/nullref.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")
#undef  xuser

namespace mixc::docker_tuple::origin{
    template<class a0 = decltype(nullptr), class ... args>
    xstruct(
        xtmpl(tuple, a0, args...),
        xprob(tuple<args...>),
        xprif(value, a0)
    )
        template<class ...>
        constexpr tuple(){}

        constexpr tuple(a0 const & first, args const & ... result) : 
            value(first), tuple<args...>(result...){
        }

        template<uxx index>
        auto & get(){
            if constexpr (index == 0){
                return value;
            }
            else{
                return tuple<args...>::template get<index - 1>();
            }
        }

        template<uxx index, class val_t>
        auto & set(val_t const & value){
            get<index>() = value;
            return this[0];
        }

        static constexpr uxx length(){
            return 1 + sizeof...(args);
        }
    $

    template<>
    xstruct(
        xspec(tuple, decltype(nullptr))
    )
        template<uxx index>
        auto & get(){
            return inc::nullref;
        }
    $
}

#endif

xexport_space(mixc::docker_tuple::origin)