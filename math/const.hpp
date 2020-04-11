#ifndef xpack_math_const
#define xpack_math_const
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_const
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_const::inner{
        constexpr f64 pi        = 3.141592653589793238462643383279502884;
        constexpr f64 e         = 2.718281828459045235360287471352662497;
        constexpr f64 sqrt_2    = 1.414213562373095048801688724209698078;
        constexpr f64 log2_e    = 1.442695040888963407359924681001892137;
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::math_const::inner;
}
