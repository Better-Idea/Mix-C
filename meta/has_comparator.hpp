#ifndef xpack_meta_has_comparator
#define xpack_meta_has_comparator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_comparator::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_comparator::origin{
    template<class object_t>
    concept has_comparator = requires(object_t a, object_t b, bool c){
        c = a == b;
        c = a != b;
        c = a >= b;
        c = a >  b;
        c = a <= b;
        c = a <  b;
    };
}

#endif

xexport_space(mixc::meta_has_comparator::origin)