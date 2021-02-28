#ifndef xpack_meta_remove_membership
#define xpack_meta_remove_membership
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_remove_membership::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_remove_membership{
    template<class object_t> struct meta{
        using result = object_t;
    };

    template<class type_t, class object_t>
    struct meta<type_t object_t::*>{
        using result = type_t;
    };

    template<class ret_t, class object_t, class ... args_t>
    struct meta<ret_t (object_t::*)(args_t...) const>{
        using result = ret_t (*)(args_t...);
    };

    template<class ret_t, class object_t, class ... args_t>
    struct meta<ret_t (object_t::* const)(args_t...)>{
        using result = ret_t (* const)(args_t...);
    };

    template<class ret_t, class ... args_t>
    struct meta<ret_t (*)(args_t...)>{
        using result = ret_t (*)(args_t...);
    };

    template<class ret_t, class ... args_t>
    struct meta<ret_t (* const)(args_t...)>{
        using result = ret_t (* const)(args_t...);
    };

    template<class type_t>
    using remove_membership = typename meta<type_t>::result;
}

#endif

xexport(mixc::meta_remove_membership::remove_membership)