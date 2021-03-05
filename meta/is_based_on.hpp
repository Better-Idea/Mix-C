#ifndef xpack_meta_is_based_on
#define xpack_meta_is_based_on
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_based_on::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_based_on{
    template<class father_t, class child_t>
    concept is_based_on = __is_base_of(father_t, child_t);
}

#endif

xexport(mixc::meta_is_based_on::is_based_on)