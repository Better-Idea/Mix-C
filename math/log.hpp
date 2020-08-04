#ifndef xpack_math_log
#define xpack_math_log
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_log
#include"define/base_type.hpp"
#include"math/const.hpp"
#include"math/ln.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_log{
    inline f64 log(f64 a, f64 x){
        return inc::ln(x) / inc::ln(a);
    }
}

#endif

namespace xuser::inc{
    using ::mixc::math_log::log;
}
