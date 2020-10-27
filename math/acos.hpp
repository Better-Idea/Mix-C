#ifndef xpack_math_acos
#define xpack_math_acos
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_acos
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_acos{
    extern f64 acos(f64 x);
}

#endif

xexport(mixc::math_acos::acos)
