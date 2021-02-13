#ifndef xpack_meta_has_logic_and
#define xpack_meta_has_logic_and
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_logic_and::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_logic_and::origin{
    template<class a>
    concept has_logic_and = requires(a left, a right){
        left & right;
    };
}

#endif

xexport_space(mixc::meta_has_logic_and::origin)