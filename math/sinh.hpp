#ifndef xpack_math_sinh
#define xpack_math_sinh
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sinh
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sinh{
    extern f64 sinh(f64 x);
}

#endif

xexport(mixc::math_sinh::sinh)
