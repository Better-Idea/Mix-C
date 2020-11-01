#ifndef xpack_math_sqrtr
#define xpack_math_sqrtr
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sqrtr::inc
#include"define/base_type.hpp"
#include"define/nan.hpp"
#include"macro/xexport.hpp"
#include"math/sqrt.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sqrtr::origin{
    // 注意：
    // 此 unsafe 函数不带定义域检查，需要使用者保证 x > 0
    // 当 x = 0 时，result -> inf
    inline f64 sqrtr_unsafe(f64 x){
        return 1.0 / inc::sqrt_unsafe(x);
    }

    inline f64 sqrtr(f64 x){
        if (x <= 0){
            return inc::nan;
        }
        return sqrtr_unsafe(x);
    }
}

#endif

xexport_space(mixc::math_sqrtr::origin)