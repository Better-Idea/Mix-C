#ifndef xpack_math_cos
#define xpack_math_cos
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_cos
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_cos{
    extern f64 cos(f64 x);
}

#endif

namespace xuser::inc{
    using ::mixc::math_cos::cos;
}
