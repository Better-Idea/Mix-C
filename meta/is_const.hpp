#ifndef xpack_meta_is_const
#define xpack_meta_is_const
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_const::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_const{
    template<class type_t> struct meta {
        static constexpr bool result = false;
    };

    template<class type_t> struct meta<const type_t> {
        static constexpr bool result = true;
    };

    template<class type_t>
    concept is_const = meta<type_t>::result;
}

#endif

xexport(mixc::meta_is_const::is_const)