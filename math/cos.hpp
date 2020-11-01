#ifndef xpack_math_cos
#define xpack_math_cos
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_cos::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/div.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_cos::origin{
    // 注意：
    // 此 unsafe 函数不带定义域检查，需要使用者保证 x 的范围属于 [-pi, +pi]
    inline f64 cos_unsafe(f64 x){
        f64 t  = inc::tan_unsafe(0.5 * x);
        f64 tt = t * t;
        return (1.0 - tt) / (1.0 + tt);
    }

    inline f64 cos(f64 x){
        x = inc::div(x, inc::pi).remainder;
        return cos_unsafe(x);
    }
}

#endif

xexport_space(mixc::math_cos::origin)