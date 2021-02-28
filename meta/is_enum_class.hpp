#ifndef xpack_meta_is_enum_class
#define xpack_meta_is_enum_class
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_enum_class::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/has_cast.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_enum_class::origin{
    template<class type_t>
    concept is_enum_class = not inc::has_cast<u64, type_t> and __is_enum(type_t);
}

#endif

xexport_space(mixc::meta_is_enum_class::origin)