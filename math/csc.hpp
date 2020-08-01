#ifndef xpack_math_csc
#define xpack_math_csc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_csc
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_csc{
    extern f64 csc(f64 x);
}

#endif

namespace xuser::inc{
    using ::mixc::math_csc::csc;
}
