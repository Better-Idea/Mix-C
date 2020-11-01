#ifndef xpack_meta_is_based_on
#define xpack_meta_is_based_on
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_based_on::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_based_on{
    template<class father, class child>
    concept is_based_on = __is_base_of(father, child);
}

#endif

xexport(mixc::meta_is_based_on::is_based_on)