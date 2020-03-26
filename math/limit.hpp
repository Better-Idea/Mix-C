#ifndef xpack_math_limit
#define xpack_math_limit
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_limit
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_limit{
        template<class a> constexpr a max = a();
        template<class a> constexpr a min = a();

        template<> constexpr i08 max<i08> = 127;
        template<> constexpr i16 max<i16> = 32767;
        template<> constexpr i32 max<i32> = 2147483647;
        template<> constexpr i64 max<i64> = 9223372036854775807;
        template<> constexpr u08 max<u08> = 255;
        template<> constexpr u16 max<u16> = 65535;
        template<> constexpr u32 max<u32> = 4294967295;
        template<> constexpr u64 max<u64> = 18446744073709551615ULL;
        template<> constexpr f32 max<f32> = 3.40282347e+38;
        template<> constexpr f64 max<f64> = 1.7976931348623157e+308;

        template<> constexpr i08 min<i08> = -128;
        template<> constexpr i16 min<i16> = -32768;
        template<> constexpr i32 min<i32> = -2147483648;
        template<> constexpr i64 min<i64> = -9223372036854775807LL - 1;
        template<> constexpr u08 min<u08> = 0;
        template<> constexpr u16 min<u16> = 0;
        template<> constexpr u32 min<u32> = 0;
        template<> constexpr u64 min<u64> = 0;
        template<> constexpr f32 min<f32> = -3.40282347e+38;
        template<> constexpr f64 min<f64> = -1.7976931348623157e+308;
    }

#endif

namespace xuser::inc{
    using ::mixc::math_limit::max;
    using ::mixc::math_limit::min;
}
