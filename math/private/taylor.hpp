#ifndef xpack_math_private_taylor
#define xpack_math_private_taylor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_private_taylor::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

// 生成 4x6 常量系数
// coe (coefficient)
#define xcoe(name)                                                  \
template<class float_t>                                             \
struct coe_ ## name{                                                \
private:                                                            \
    static constexpr float_t value(uxx n);                          \
public:                                                             \
    using float_type = float_t;                                     \
    static constexpr float_t lut[] = {                              \
        value(0x00), value(0x01), value(0x02), value(0x03),         \
        value(0x04), value(0x05), value(0x06), value(0x07),         \
        value(0x08), value(0x09), value(0x0a), value(0x0b),         \
        value(0x0c), value(0x0d), value(0x0e), value(0x0f),         \
        value(0x10), value(0x11), value(0x12), value(0x13),         \
        value(0x14), value(0x15), value(0x16), value(0x17),         \
    };                                                              \
    static constexpr uxx length = sizeof(lut) / sizeof(lut[0]);     \
};                                                                  \
template<class float_t>                                             \
constexpr float_t coe_ ## name<float_t>::value(uxx n)

namespace mixc::math_private_taylor::imm{
    template<class type>
    inline constexpr type factorial(type n){
        if (n <= 1){
            return 1;
        }
        return n * factorial(n - 1);
    }

    template<class type>
    inline constexpr type pow(type x, uxx n){
        if (n == 0){
            return 1;
        }
        if (n == 1){
            return x;
        }
        else{
            return x * pow(x, n - 1);
        }
    }

    template<class type>
    // cumulative product
    inline constexpr type cum(type x, uxx n, uxx step){
        if (n == 0){
            return 1;
        }
        if (n == 1){
            return x;
        }
        return x * cum(x + step, n - 1, step);
    }
}

namespace mixc::math_private_taylor::origin{
    template<class lut_t>
    inline typename lut_t::float_type taylor(
        typename lut_t::float_type x0, 
        typename lut_t::float_type x1, 
        typename lut_t::float_type x2, 
        typename lut_t::float_type x3, 
        typename lut_t::float_type mx){

        auto s0 = typename lut_t::float_type(0);
        auto s1 = typename lut_t::float_type(0);
        auto s2 = typename lut_t::float_type(0);
        auto s3 = typename lut_t::float_type(0);

        static_assert(lut_t::length % 4 == 0);

        for(uxx i = 0; i < lut_t::length; i += 4){
            s0 += lut_t::lut[0x0 + i] * x0;
            s1 += lut_t::lut[0x1 + i] * x1;
            s2 += lut_t::lut[0x2 + i] * x2;
            s3 += lut_t::lut[0x3 + i] * x3;
            x0 *= mx;
            x1 *= mx;
            x2 *= mx;
            x3 *= mx;
        }
        s0 += s2;
        s1 += s3;
        s0 += s1;
        return s0;
    }
}

#endif

xexport_space(mixc::math_private_taylor::origin)
xexport_spacex(adv, mixc::math_private_taylor::imm)
