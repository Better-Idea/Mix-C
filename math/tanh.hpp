#ifndef xpack_math_tanh
#define xpack_math_tanh
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_tanh::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_tanh::origin{
    extern f64 tanh(f64 x);
}

#endif

xexport_space(mixc::math_tanh::origin)
