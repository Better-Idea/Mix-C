#ifndef xpack_math_sec
#define xpack_math_sec
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sec::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/div.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sec::origin{
    // 注意：
    // 此 unsafe 函数不带定义域检查，需要使用者保证 x 的范围属于 [-pi, +pi]
    // 当 x = pi/2 时， result -> inf
    inline f64 sec_unsafe(f64 x){
        f64 t  = inc::tan_unsafe(0.5 * x);
        f64 tt = t * t;
        return (1.0 + tt) / (1.0 - tt);
    }

    inline f64 sec(f64 x){
        x = inc::div(x, inc::pi).remainder;
        return sec_unsafe(x);
    }
}

#endif

xexport_space(mixc::math_sec::origin)