#ifndef xpack_math_ln
#define xpack_math_ln
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_ln
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_ln{
    extern f64 ln(f64 x);
}

#endif

namespace xuser::inc{
    using ::mixc::math_ln::ln;
}
