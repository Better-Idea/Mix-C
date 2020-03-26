#ifndef xpack_instruction_bit_test
#define xpack_instruction_bit_test
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_bit_test
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_bit_test{
        #define xgen(type)                                              \
            inline bool bit_test(type value, uxx index){                \
                return (value & (type(1) << index)) != 0;               \
            }                                                           \

        xgen(u64);
        xgen(u32);
        xgen(u16);
        xgen(u08);
        #undef xgen
    }
#endif

namespace xuser::inc{
    using ::mixc::instruction_bit_test::bit_test;
}
