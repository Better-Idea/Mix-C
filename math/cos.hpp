#ifndef xpack_math_cos
#define xpack_math_cos
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_cos::inc
#include"define/base_type.hpp"
#include"docker/array.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"math/private/parallel_lut.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_cos{
    template<class type, uxx multiple>
    constexpr auto lut = inc::parallel_lut<multiple>(
        type(-5.00000000000000000e-01),
        type(+4.16666666666666644e-02),
        type(-1.38888888888888873e-03),
        type(+2.48015873015872982e-05),
        type(-2.75573192239858883e-07),
        type(+2.08767569878680961e-09),
        type(-1.14707455977297245e-11),
        type(+4.77947733238738525e-14),
        type(-1.56192069685862277e-16),
        type(+4.11031762331216533e-19)
    );
}

namespace mixc::math_cos::origin{
    template<class type, class ... args>
    inline auto cos(type x0, args ... xn) {
        constexpr auto multiple = 1 + sizeof...(args);
        constexpr auto step = sizeof(type) == sizeof(f32) ? 6 : 10;

        auto && x2 = inc::array_view{x0, xn...};
        auto && xm = inc::array_view{x0, xn...};
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

xexport_space(mixc::math_cos::origin)