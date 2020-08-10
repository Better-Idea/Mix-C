#ifndef xpack_math_cos
#define xpack_math_cos
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_cos
#include"define/base_type.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_cos::origin{
    inline f64 cos_unsafe(f64 x){
        f64 t  = inc::tan_unsafe(0.5 * x);
        f64 tt = t * t;
        return (1.0 - tt) / (1 + tt);
    }
}

#endif

namespace xuser::adv{
    using namespace ::mixc::math_cos::origin;
}
