#ifndef xpack_math_ln
#define xpack_math_ln
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_ln::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"define/nan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_ln::origin{
    extern f64 ln_unsafe(f64 x);

    inline f64 ln(f64 x){
        if (x <= 0){
            return inc::nan;
        }
        return ln_unsafe(x);
    }
}

#endif

xexport_space(mixc::math_ln::origin)