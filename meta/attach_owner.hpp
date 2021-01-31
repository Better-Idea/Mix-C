#ifndef xpack_meta_attach_owner
#define xpack_meta_attach_owner
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_attach_owner::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_attach_owner{
    template<class object, class func> struct meta;

    template<class object, class ret, class ... args>
    struct meta<object, ret(args...)>{
        using type = ret(object::*)(args...);
    };

    template<class object, class func> 
    using attach_owner = typename meta<object, func>::type;
}

#endif

xexport(mixc::meta_attach_owner::attach_owner)