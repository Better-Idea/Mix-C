#ifndef xpack_math_sin
#define xpack_math_sin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sin
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sin{
    extern f64 sin(f64 x);
}

#endif

namespace xuser::inc{
    using ::mixc::math_sin::sin;
}
