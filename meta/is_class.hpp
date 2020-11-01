#ifndef xpack_meta_is_class
#define xpack_meta_is_class
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_class::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_class{
    template<class a>
    concept is_class = __is_class(a);
}

#endif

xexport(mixc::meta_is_class::is_class)