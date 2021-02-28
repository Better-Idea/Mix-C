#ifndef xpack_meta_has_logic_not
#define xpack_meta_has_logic_not
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_logic_not::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_logic_not::origin{
    template<class type_t>
    concept has_logic_not = requires(type_t left){
        ~left;
    };
}

#endif

xexport_space(mixc::meta_has_logic_not::origin)