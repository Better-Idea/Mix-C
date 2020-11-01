#ifndef xpack_math_log10
#define xpack_math_log10
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_log10::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/ln.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_log10::origin{
    inline f64 log10_unsafe(f64 x){
        return inc::ln_unsafe(x) * inc::lnr_10; // 等效除以 ln_10
    }

    inline f64 log10(f64 x){
        return inc::ln(x) * inc::lnr_10; // 等效除以 ln_10
    }
}

#endif

xexport_space(mixc::math_log10::origin)