#ifndef xpack_math_sin
#define xpack_math_sin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sin::inc
#include"define/base_type.hpp"
#include"docker/array.hpp"
#include"macro/xalign.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/private/parallel_lut.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sin{
    template<class type_t, uxx multiple>
    static inline xalign(32) auto lut = inc::parallel_lut<multiple>(
        type_t(+1.00000000000000000e+00),
        type_t(-1.66666666666666657e-01),
        type_t(+8.33333333333333322e-03),
        type_t(-1.98412698412698413e-04),
        type_t(+2.75573192239858925e-06),
        type_t(-2.50521083854417202e-08),
        type_t(+1.60590438368216133e-10),
        type_t(-7.64716373181981641e-13),
        type_t(+2.81145725434552060e-15),
        type_t(-8.22063524662432950e-18)
    );

    static inline auto lut_f64x1 = (f64 *)lut<f64, 1>;
}

namespace mixc::math_sin::origin{
    template<class type_t, class ... args_t>
    inline auto sin_unsafe(type_t x0, args_t ... xn) {
        constexpr auto multiple = 1 + sizeof...(args_t);
        constexpr auto step     = sizeof(type_t) == sizeof(f32) ? 6 : 10;

        auto && x2 = inc::array_view{x0, xn...};
        auto && xm = inc::array_view{x0, xn...};
        auto && rx = inc::array<type_t, multiple>();

        for(uxx i = 0; i < multiple; i++){
            x2[i] *= x2[i];
        }

        for(uxx i = 0; i < step; i++){
            for(uxx j = 0; j < multiple; j++){
                rx[j] += xm[j] * lut<type_t, multiple>[i * multiple + j];
                xm[j] *= x2[j];
            }
        }

        if constexpr (multiple == 1){
            return rx[0];
        }
        else{
            return rx;
        }
    }

    template<class type_t, class ... args_t>
    inline auto sin(type_t x0, args_t ... xn) {
        auto normalize = [](type_t x){
            constexpr auto p  = type_t(inc::pi);
            constexpr auto rp = type_t(1.0) / p;
            constexpr auto hp = 0.5 * p;
            x          += hp;

            auto q      = i64(x * rp);          // 整数倍
            auto r      = type_t(x - q * p);    // 余数
            auto adjust = i64(q) & 1 ? 
                p - r:                          // 奇数倍 
                type_t(r);                      // 偶数倍
            return adjust - hp;
        };
        return sin_unsafe(normalize(x0), normalize(xn)...);
    }
}

#endif

xexport_space(mixc::math_sin::origin)