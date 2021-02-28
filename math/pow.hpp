#ifndef xpack_math_pow
#define xpack_math_pow
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_pow::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_pow{
    template<class type_t>
    inline type_t pow(type_t a, uxx x){
        type_t r       = 1;

        for(; x != 0; x >>= 1, a *= a){
            if (x & 1){
                r *= a;
            }
        }
        return r;
    }
}

#endif

xexport(mixc::math_pow::pow)