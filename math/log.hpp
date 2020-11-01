#ifndef xpack_math_log
#define xpack_math_log
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_log::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/ln.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_log::origin{
    inline f64 log_unsafe(f64 a, f64 x){
        return inc::ln_unsafe(x) / inc::ln_unsafe(a);
    }

    inline f64 log(f64 a, f64 x){
        return inc::ln(x) / inc::ln(a);
    }
}

#endif

xexport_space(mixc::math_log::origin)