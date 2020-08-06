#ifndef xpack_math_sin
#define xpack_math_sin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sin
#include"define/base_type.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sin::origin{
    inline f64 sin_unsafe(f64 x){
        f64 t = inc::tan_unsafe(0.5 * x);
        return 2.0 * t / (1.0 + t * t);
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::math_sin::origin;
}
