#ifndef xpack_math_round
#define xpack_math_round
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_round
#include"math/div.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_round{
    inline f32 round_unsafe(f32 a){
        return adv::div_unsafe(a + 0.5, 1).quotient;
    }

    inline f64 round_unsafe(f64 a){
        return adv::div_unsafe(a + 0.5, 1).quotient;
    }

    inline f80 round_unsafe(f80 a){
        return adv::div_unsafe(a + 0.5, 1).quotient;
    }

    inline f32 round(f32 a){
        return inc::div(a + 0.5, 1).quotient;
    }

    inline f64 round(f64 a){
        return inc::div(a + 0.5, 1).quotient;
    }

    inline f80 round(f80 a){
        return inc::div(a + 0.5, 1).quotient;
    }
}

#endif

xexport(mixc::math_round::round)
xexport_adv(mixc::math_round::round_unsafe)
