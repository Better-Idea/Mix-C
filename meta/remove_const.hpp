#ifndef xpack_meta_remove_const
#define xpack_meta_remove_const
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_remove_const::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_remove_const{
    template<class type_t>
    struct meta {
        using result = type_t;
    };

    template<class type_t>
    struct meta<const type_t> {
        using result = type_t;
    };

    template<class type>
    using remove_const = typename meta<type>::result;
}

#endif

xexport(mixc::meta_remove_const::remove_const)