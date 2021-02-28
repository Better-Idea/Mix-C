#ifndef xpack_meta_is_origin_array
#define xpack_meta_is_origin_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_origin_array::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_origin_array{
    template<class>
    struct meta{ enum { result = false }; };

    template<class type_t, uxx length_v>
    struct meta<type_t[length_v]>{ enum { result = true }; };

    template<class type_t>
    concept is_origin_array = bool(meta<type_t>::result);
}

#endif

xexport(mixc::meta_is_origin_array::is_origin_array)