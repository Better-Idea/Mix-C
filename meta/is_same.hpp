#ifndef xpack_meta_is_same
#define xpack_meta_is_same
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_same::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_same{
    template<class a0_t, class a1_t> 
    struct meta {
        static constexpr bool result = false;
    };

    template<class a0_t>
    struct meta<a0_t, a0_t> {
        static constexpr bool result = true;
    };

    template<class a0_t, class a1_t>
    constexpr bool is_same = meta<a0_t, a1_t>::result;
}

#endif

xexport(mixc::meta_is_same::is_same)