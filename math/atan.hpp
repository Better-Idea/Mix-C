#ifndef xpack_math_atan
#define xpack_math_atan
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_atan
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_atan{
    extern f64 atan(f64 x);
}

#endif

xexport(mixc::math_atan::atan)
