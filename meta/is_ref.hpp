#ifndef xpack_meta_is_ref
#define xpack_meta_is_ref
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_ref::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_ref{
    template<class a> struct meta {
        static constexpr bool result = false;
    };

    template<class a> struct meta<a &> {
        static constexpr bool result = true;
    };

    template<class a>
    concept is_ref = meta<a>::result;
}

#endif

xexport(mixc::meta_is_ref::is_ref)