#ifndef xpack_math_asin
#define xpack_math_asin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_asin::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_asin{
    extern f64 asin(f64 x);
}

#endif

xexport(mixc::math_asin::asin)
