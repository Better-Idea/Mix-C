#ifndef xpack_meta_is_non_member_callback
#define xpack_meta_is_non_member_callback
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_non_member_callback::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_lambda.hpp"
#include"meta/is_static_func.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_non_member_callback{
    template<class type_t>
    concept is_non_member_callback = 
        inc::is_lambda<type_t> or inc::is_static_func<type_t>;
}

#endif

xexport(mixc::meta_is_non_member_callback::is_non_member_callback)