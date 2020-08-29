#ifndef xpack_math_pow
#define xpack_math_pow
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_pow
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_pow{
    template<class type>
    inline type pow(type a, uxx x){
        type r       = 1;

        for(; x != 0; x >>= 1, a *= a){
            if (x & 1){
                r *= a;
            }
        }
        return r;
    }
}

#endif

namespace xuser::inc{
    using ::mixc::math_pow::pow;
}
