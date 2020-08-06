#ifndef xpack_math_sec
#define xpack_math_sec
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sec
#include"define/base_type.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sec::origin{
    inline f64 sec_unsafe(f64 x){
        f64 t  = inc::tan_unsafe(0.5 * x);
        f64 tt = t * t;
        return (1 + tt) / (1.0 - tt);
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::math_sec::origin;
}
