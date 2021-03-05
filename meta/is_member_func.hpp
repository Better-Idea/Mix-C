#ifndef xpack_meta_is_member_func
#define xpack_meta_is_member_func
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_member_func::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_member_func{
    template<class object_t, class ret_t, class ... args_t>
    inline constexpr bool invoke(ret_t(object_t::*)(args_t...) const){
        return true;
    }

    template<class object_t, class ret_t, class ... args_t>
    inline constexpr bool invoke(ret_t(object_t::*)(args_t...)){
        return true;
    }

    template<class other_t>
    inline constexpr bool invoke(other_t *){
        return false;
    }

    template<class type_t>
    concept is_member_func = invoke((type_t)nullptr);
}

#endif

xexport(mixc::meta_is_member_func::is_member_func)