#ifndef xpack_math_numeration_t
#define xpack_math_numeration_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_numeration_t
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_numeration_t{
        enum class numeration_t : u08 {
            bin =  2,
            oct =  8,
            dec =  10,
            hex =  16,
        };

        namespace numeration{
            constexpr numeration_t bin = numeration_t::bin;
            constexpr numeration_t oct = numeration_t::oct;
            constexpr numeration_t dec = numeration_t::dec;
            constexpr numeration_t hex = numeration_t::hex;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::math_numeration_t::numeration_t;

    namespace numeration{
        using namespace ::mixc::math_numeration_t::numeration;
    }
}
