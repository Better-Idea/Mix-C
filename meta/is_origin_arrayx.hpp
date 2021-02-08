#ifndef xpack_meta_is_origin_arrayx
#define xpack_meta_is_origin_arrayx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_origin_arrayx::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_origin_arrayx{
    template<class, class>
    struct meta{ enum { result = false }; };

    template<class a, uxx length>
    struct meta<a[length], a>{ enum { result = true }; };

    template<class a, class item_t>
    concept is_origin_arrayx = bool(meta<a, item_t>::result);
}

#endif

xexport(mixc::meta_is_origin_arrayx::is_origin_arrayx)