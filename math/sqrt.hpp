#ifndef xpack_math_sqrt
#define xpack_math_sqrt
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sqrt
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sqrt{
    inline f64 sqrt_unsafe(f64 x);
}

#endif

namespace xuser::adv{
    using ::mixc::math_sqrt::sqrt_unsafe;
}
