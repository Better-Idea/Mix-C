#ifndef xpack_math_ln
#define xpack_math_ln
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_ln
#include"define/base_type.hpp"
#include"define/nan.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_ln{
    extern f64 ln_unsafe(f64 x);

    inline f64 ln(f64 x){
        if (x <= 0){
            return inc::nan;
        }
        return ln_unsafe(x);
    }
}

#endif

namespace xuser::inc{
    using ::mixc::math_ln::ln;
}

namespace xuser::adv{
    using ::mixc::math_ln::ln_unsafe;
}
