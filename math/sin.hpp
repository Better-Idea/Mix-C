#ifndef xpack_math_sin
#define xpack_math_sin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sin
#include"define/base_type.hpp"
#include"math/const.hpp"
#include"math/div.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sin{
    // 注意：
    // 此 unsafe 函数不带定义域检查，需要使用者保证 x 的范围属于 [-pi, +pi]
    inline f64 sin_unsafe(f64 x){
        f64 t = adv::tan_unsafe(0.5 * x);
        return 2.0 * t / (1.0 + t * t);
    }

    inline f64 sin(f64 x){
        x = inc::div(x, inc::pi).remainder;
        return sin_unsafe(x);
    }
}

#endif

namespace xuser::inc{
    using ::mixc::math_sin::sin;
}

namespace xuser::adv{
    using ::mixc::math_sin::sin_unsafe;
}
