#ifndef xpack_math_exp10
#define xpack_math_exp10
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::math_exp10
    #include"mixc.hpp"
    #include"define/nan.hpp"

    namespace xuser{
        constexpr f64 lut_1e0_1e15[]     = { 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, };
        constexpr f64 lut_1e16_1e240[]   = { 1e0, 1e16, 1e32, 1e48, 1e64, 1e80, 1e96, 1e112, 1e128, 1e144, 1e160, 1e176, 1e192, 1e208, 1e224, 1e240};
        constexpr f64 lut_1e256[]        = { 1e0, 1e256 };

        inline f64 exp10(uxx x){
            xdebug_fail(x > 307){
                return inc::nan;
            }

            union {
                struct{
                    uxx low : 4;
                    uxx mid : 4;
                    uxx hig : 1;
                };
                uxx full;
            };

            full    = x;
            return lut_1e0_1e15[low] * lut_1e16_1e240[mid] * lut_1e256[hig];
        }
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::math_exp10::exp10;
}
