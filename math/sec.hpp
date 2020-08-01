#ifndef xpack_math_sec
#define xpack_math_sec
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sec
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sec{
    extern f64 sec(f64 x);
}

#endif

namespace xuser::inc{
    using ::mixc::math_sec::sec;
}
