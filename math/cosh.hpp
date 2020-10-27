#ifndef xpack_math_cosh
#define xpack_math_cosh
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_cosh
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_cosh{
    extern f64 cosh(f64 x);
}

#endif

xexport(mixc::math_cosh::cosh)
