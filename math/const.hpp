#ifndef xpack_math_const
#define xpack_math_const
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_const
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_const::origin{
    inline constexpr f64 pi        = 3.141592653589793238463;
    inline constexpr f64 e         = 2.718281828459045235360;
    inline constexpr f64 sqrt_2    = 1.414213562373095048802;
    inline constexpr f64 sqrtr_2   = 1.0 / sqrt_2;
    inline constexpr f64 log2_e    = 1.442695040888963407360;
    inline constexpr f64 log2_10   = 3.321928094887362347810;
    inline constexpr f64 ln_2      = 0.693147180559945309417;
    inline constexpr f64 log10_2   = 0.301029995663981195226;

    template<class a> inline constexpr a max_value_of = a();
    template<class a> inline constexpr a min_value_of = a();

    template<> inline constexpr i08 max_value_of<i08> = 127;
    template<> inline constexpr i16 max_value_of<i16> = 32767;
    template<> inline constexpr i32 max_value_of<i32> = 2147483647;
    template<> inline constexpr i64 max_value_of<i64> = 9223372036854775807;
    template<> inline constexpr u08 max_value_of<u08> = 255;
    template<> inline constexpr u16 max_value_of<u16> = 65535;
    template<> inline constexpr u32 max_value_of<u32> = 4294967295;
    template<> inline constexpr u64 max_value_of<u64> = 18446744073709551615ULL;
    template<> inline constexpr f32 max_value_of<f32> = f32(3.40282347e+38);
    template<> inline constexpr f64 max_value_of<f64> = f64(1.7976931348623157e+308);

    template<> inline constexpr i08 min_value_of<i08> = -128;
    template<> inline constexpr i16 min_value_of<i16> = -32768;
    template<> inline constexpr i32 min_value_of<i32> = -2147483648;
    template<> inline constexpr i64 min_value_of<i64> = -9223372036854775807LL - 1;
    template<> inline constexpr u08 min_value_of<u08> = 0;
    template<> inline constexpr u16 min_value_of<u16> = 0;
    template<> inline constexpr u32 min_value_of<u32> = 0;
    template<> inline constexpr u64 min_value_of<u64> = 0;
    template<> inline constexpr f32 min_value_of<f32> = f32(-3.40282347e+38);
    template<> inline constexpr f64 min_value_of<f64> = f64(-1.7976931348623157e+308);
}

#endif

namespace xuser::inc{
    using namespace ::mixc::math_const::origin;
}
