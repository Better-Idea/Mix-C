#ifndef xpack_meta_attach_owner
#define xpack_meta_attach_owner
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_attach_owner::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_attach_owner{
    template<class object_t, class func_t> struct meta;

    template<class object_t, class ret_t, class ... args_t>
    struct meta<object_t, ret_t(args_t...)>{
        using type = ret_t(object_t::*)(args_t...);
    };

    template<class object_t, class func_t> 
    using attach_owner = typename meta<object_t, func_t>::type;
}

#endif

xexport(mixc::meta_attach_owner::attach_owner)