#ifndef xpack_meta_first
#define xpack_meta_first
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_first::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_first{
    template<class ... args> struct meta;

    template<class arg0, class ... rest>
    struct meta<arg0, rest...>{
        using result = arg0;
    };

    template<>
    struct meta<>{
        using result = void;
    };

    template<class ... args>
    using first = typename meta<args...>::result;
}

#endif

xexport(mixc::meta_first::first)