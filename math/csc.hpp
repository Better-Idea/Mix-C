#ifndef xpack_math_csc
#define xpack_math_csc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_csc
#include"define/base_type.hpp"
#include"math/const.hpp"
#include"math/div.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_csc{
    // 注意：
    // 此 unsafe 函数不带定义域检查，需要使用者保证 x 的范围属于 [-pi, +pi]
    // 当 x = 0 时， result -> inf
    inline f64 csc_unsafe(f64 x){
        f64 t = adv::tan_unsafe(0.5 * x);
        return (1.0 + t * t) / (2.0 * t);
    }

    inline f64 csc(f64 x){
        x = inc::div(x, inc::pi).remainder;
        return csc_unsafe(x);
    }
}

#endif

namespace xuser::inc{
    using ::mixc::math_csc::csc;
}

namespace xuser::adv{
    using ::mixc::math_csc::csc_unsafe;
}
