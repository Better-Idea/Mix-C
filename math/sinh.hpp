#ifndef xpack_math_sinh
#define xpack_math_sinh
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sinh::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sinh::origin{
    extern f64 sinh(f64 x);
}

#endif

xexport_space(mixc::math_sinh::origin)
