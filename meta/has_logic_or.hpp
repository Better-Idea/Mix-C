#ifndef xpack_meta_has_logic_or
#define xpack_meta_has_logic_or
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_logic_or::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_logic_or::origin{
    template<class type_t>
    concept has_logic_or = requires(type_t left, type_t right){
        left | right;
    };
}

#endif

xexport_space(mixc::meta_has_logic_or::origin)