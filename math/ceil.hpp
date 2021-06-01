#ifndef xpack_math_ceil
#define xpack_math_ceil
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_ceil::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/abs.hpp"
#include"utils/mfxx.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_ceil{
    template<class type_t>
    inline type_t ceil(type_t value){
        if (value == 0){
            return value;
        }

        using mfxx              = inc::mfxx<type_t>;
        auto v                  = mfxx{ value };
        auto positive           = mfxx{ inc::abs(value) };
        auto exp                = positive.real_exp_unsafe();
        auto dec                = positive.real_dec_unsafe();
        constexpr
        auto precious           = ixx(positive.bits_of_mantissa());

        // 如果数值不含小数部分
        if (exp >= precious){
            return value;
        }
        if (exp < 0){
            return type_t(value < 0 ? 0 : 1);
        }

        // 判断小数位是否为 0
        // 不为 0 就圆整
        auto i                  = u64(1) << (precious - exp);
        auto mask               = i - 1;

        // 正浮点数[尾数]向上圆整，需要 + mask
        // 负浮点数[尾数]向下圆整，无需 + mask
        if (value > 0) {
            dec                 = dec + mask;
        }

        dec                    &= ~mask;
        v.decimal               = dec;

        // 溢出
        if (value > 0 and v.decimal == 0 and positive.decimal != 0){
            v                  *= 2;
        }
        return v;
    }
}

namespace mixc::math_ceil::origin{
    inline f64 ceil(f64 value){
        return mixc::math_ceil::ceil(value);
    }

    inline f32 ceil(f32 value){
        return mixc::math_ceil::ceil(value);
    }
}

#endif

xexport_space(mixc::math_ceil::origin)