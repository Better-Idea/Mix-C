#ifndef xpack_math_expr10
#define xpack_math_expr10
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::math_expr10
    #include"mixc.hpp"
    #include"define/nan.hpp"

    namespace xuser{
        constexpr f64 lut_1en0_1en15[]   = { 1e-0, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9, 1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, };
        constexpr f64 lut_1en16_1en240[] = { 1e-0, 1e-16, 1e-32, 1e-48, 1e-64, 1e-80, 1e-96, 1e-112, 1e-128, 1e-144, 1e-160, 1e-176, 1e-192, 1e-208, 1e-224, 1e-240,};
        constexpr f64 lut_1en256[]       = { 1e0, 1e-256 };

        inline f64 expr10(uxx x){
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
            return lut_1en0_1en15[low] * lut_1en16_1en240[mid] * lut_1en256[hig];
        }
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::math_expr10::expr10;
}
