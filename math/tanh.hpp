#ifndef xpack_math_tanh
#define xpack_math_tanh
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_tanh
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_tanh{
    extern f64 tanh(f64 x);
}

#endif

xexport(mixc::math_tanh::tanh)
