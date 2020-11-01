#ifndef xpack_math_tan
#define xpack_math_tan
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_tan::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/div.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_tan::origin{
    // 注意：
    // 此 unsafe 函数不带定义域检查，需要使用者保证 x 的范围属于 [-pi/2, +pi/2]
    // 当 x = +-pi/2 时， result -> +-inf
    extern f64 tan_unsafe(f64 x);

    inline f64 tan(f64 x){
        x = inc::div(x, inc::pi / 2).remainder;
        return tan_unsafe(x);
    }
}

#endif

xexport_space(mixc::math_tan::origin)