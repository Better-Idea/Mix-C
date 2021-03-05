#ifndef xpack_meta_is_func
#define xpack_meta_is_func
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_func::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_member_func.hpp"
#include"meta/is_static_func.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_func{
    template<class type_t>
    concept is_func = 
        inc::is_member_func<type_t> or 
        inc::is_static_func<type_t>;
}

#endif

xexport(mixc::meta_is_func::is_func)