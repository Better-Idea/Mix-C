#pragma once
#ifndef xpack_meta_is_volatile
#define xpack_meta_is_volatile
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_volatile::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_volatile{
    template<class type_t> struct meta {
        static constexpr bool result = false;
    };

    template<class type_t> struct meta<volatile type_t> {
        static constexpr bool result = true;
    };

    template<class type_t>
    concept is_volatile = meta<type_t>::result;
}

#endif

xexport(mixc::meta_is_volatile::is_volatile)