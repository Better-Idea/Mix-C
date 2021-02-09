#ifndef xpack_meta_has_forced_cast
#define xpack_meta_has_forced_cast
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_forced_cast::inc
#include"configure.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_forced_cast{
    template<class target, class source>
    concept has_forced_cast = requires(target tar, source src){
        tar = (target)src;
    };
}

#endif

xexport(mixc::meta_has_forced_cast::has_forced_cast)