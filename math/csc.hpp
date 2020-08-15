#ifndef xpack_math_csc
#define xpack_math_csc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_csc
#include"define/base_type.hpp"
#include"math/tan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_csc::origin{
    inline f64 csc_unsafe(f64 x){
        f64 t = adv::tan_unsafe(0.5 * x);
        return (1.0 + t * t) / (2.0 * t);
    }
}

#endif

namespace xuser::adv{
    using namespace ::mixc::math_csc::origin;
}
