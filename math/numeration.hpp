#ifndef xpack_math_numeration
#define xpack_math_numeration
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_numeration
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_numeration{
        enum class numeration{
            bin =  2,
            oct =  8,
            dec =  10,
            hex =  16,
        };

        constexpr numeration bin = numeration::bin;
        constexpr numeration oct = numeration::oct;
        constexpr numeration dec = numeration::dec;
        constexpr numeration hex = numeration::hex;
    }

#endif

namespace xuser::inc{
    using ::mixc::math_numeration::numeration;
    using ::mixc::math_numeration::bin;
    using ::mixc::math_numeration::oct;
    using ::mixc::math_numeration::dec;
    using ::mixc::math_numeration::hex;
}
