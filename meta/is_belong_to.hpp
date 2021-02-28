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
    template<class holder_t, class member_ptr_t>
    struct meta{
        enum{ result = 0 };
    };

    template<class holder_t, class type>
    struct meta<holder_t, type holder_t::*>{
        enum{ result = 1 };
    };

    template<class holder_t, auto member_ptr_t>
    concept is_belong_to = meta<holder_t, decltype(member_ptr_t)>::result != 0;
}

#endif

xexport(mixc::meta_is_belong_to::is_belong_to)