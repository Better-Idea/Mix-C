#ifndef xpack_meta_is_integer_based
#define xpack_meta_is_integer_based
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_integer_based::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_enum.hpp"
#include"meta/is_integer.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_integer_based{
    template<class type_t>
    concept is_integer_based = inc::is_enum<type_t> and inc::is_integer<type_t>;
}

#endif

xexport(mixc::meta_is_integer_based::is_integer_based)