#ifndef xpack_meta_is_belong_to
#define xpack_meta_is_belong_to
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_belong_to::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_belong_to{
    template<class holder, class member_ptr>
    struct meta{
        enum{ result = 0 };
    };

    template<class holder, class type>
    struct meta<holder, type holder::*>{
        enum{ result = 1 };
    };

    template<class holder, auto member_ptr>
    concept is_belong_to = meta<holder, decltype(member_ptr)>::result;
}

#endif

xexport(mixc::meta_is_belong_to::is_belong_to)