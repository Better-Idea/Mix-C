#ifndef xpack_meta_has_cmp_equal
#define xpack_meta_has_cmp_equal
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_cmp_equal::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_cmp_equal::origin{
    template<class object_t>
    concept has_cmp_equal = requires(object_t a, object_t b, bool c){
        c = a == b;
    };
}

#endif

xexport_space(mixc::meta_has_cmp_equal::origin)