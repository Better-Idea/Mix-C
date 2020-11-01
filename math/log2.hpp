#ifndef xpack_math_log2
#define xpack_math_log2
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_log2::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/ln.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_log2::origin{
    inline f64 log2_unsafe(f64 x){
        return inc::ln_unsafe(x) * inc::lnr_2; // 等效除以 ln_2
    }

    inline f64 log2(f64 x){
        return inc::ln(x) * inc::lnr_2; // 等效除以 ln_2
    }
}

#endif

xexport_space(mixc::math_log2::origin)