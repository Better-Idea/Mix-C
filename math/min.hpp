#ifndef xpack_math_min
#define xpack_math_min
#ifdef min
    #undef min
#endif

#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_min::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_min::origin{
    template<class type>
    inline type min(type left, type right){
        return left < right ? left : right;
    }
}

#endif

xexport_space(mixc::math_min::origin)