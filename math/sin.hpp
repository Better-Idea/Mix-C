#ifndef xpack_math_sin
#define xpack_math_sin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_sin::inc
#include"define/base_type.hpp"
#include"docker/array.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/private/parallel_lut.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_sin{
    template<class type, uxx multiple>
    constexpr auto lut = inc::parallel_lut<multiple>(
        type(+1.00000000000000000e+00),
        type(-1.66666666666666657e-01),
        type(+8.33333333333333322e-03),
        type(-1.98412698412698413e-04),
        type(+2.75573192239858925e-06),
        type(-2.50521083854417202e-08),
        type(+1.60590438368216133e-10),
        type(-7.64716373181981641e-13),
        type(+2.81145725434552060e-15),
        type(-8.22063524662432950e-18)
    );
}

namespace mixc::math_sin::origin{
    template<class type, class ... args>
    inline auto sin(type x0, args ... xn) {
        constexpr auto multiple = 1 + sizeof...(args);
        constexpr auto step = sizeof(type) == sizeof(f32) ? 6 : 10;

        auto && x2 = inc::array_view{x0, xn...};
        auto && xm = inc::array<type, multiple>();
        auto && rx = inc::array<type, multiple>();

        x2.foreach([](type & value){
            value *= value;
        });

        xm = x2;

        for(uxx i = 0; i < step; i++){
            for(uxx j = 0; j < multiple; j++){
                rx[j] += xm[j] * lut<type, multiple>[i * multiple + j];
                xm[j] *= x2[j];
            }
        }
        return rx;
    }
}

#endif

xexport_space(mixc::math_sin::origin)