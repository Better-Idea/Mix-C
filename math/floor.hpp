#ifndef xpack_math_floor
#define xpack_math_floor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_floor::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/ceil.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_floor::origin{
    template<class type_t>
    inline type_t floor(type_t value){
        auto ceil = inc::ceil(value);
        return ceil == value ? ceil : ceil - 1;
    }
}

#endif

xexport_space(mixc::math_floor::origin)