#ifndef xpack_meta_has_cast
#define xpack_meta_has_cast
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_cast::inc
#include"configure.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_cast{
    template<class target, class source>
    concept has_cast = requires(target tar, source src){
        tar = src;
    };
}

#endif

xexport(mixc::meta_has_cast::has_cast)