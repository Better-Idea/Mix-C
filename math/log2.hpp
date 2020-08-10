#ifndef xpack_math_log2
#define xpack_math_log2
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_log2
#include"define/base_type.hpp"
#include"math/const.hpp"
#include"math/ln.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_log2{
    inline f64 log2_unsafe(f64 x){
        return adv::ln_unsafe(x) * inc::lnr_2; // 等效除以 ln_2
    }

    inline f64 log2(f64 x){
        return inc::ln(x) * inc::lnr_2; // 等效除以 ln_2
    }
}

#endif

namespace xuser::inc{
    using ::mixc::math_log2::log2;
}

namespace xuser::adv{
    using ::mixc::math_log2::log2_unsafe;
}
