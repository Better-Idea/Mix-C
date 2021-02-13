#ifndef xpack_meta_is_enum_origin
#define xpack_meta_is_enum_origin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_enum_origin::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/has_cast.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_enum_origin{
    template<class a>
    concept is_enum_origin = inc::has_cast<u64, a> and __is_enum(a);
}

#endif

xexport(mixc::meta_is_enum_origin::is_enum_origin)