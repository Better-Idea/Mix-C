#ifndef xpack_instruction_bit_test_and_set
#define xpack_instruction_bit_test_and_set
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_bit_test_and_set
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_bit_test_and_set{
        #define xgen(type)                                              \
            inline bool bit_test_and_set(type * value, uxx index){      \
                type mask   = (type(1) << index);                       \
                bool result = (value[0] & mask) != 0;                   \
                value[0]   |= mask;                                     \
                return result;                                          \
            }                                                           \

        xgen(u64);
        xgen(u32);
        xgen(u16);
        xgen(u08);
        #undef xgen
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_bit_test_and_set::bit_test_and_set;
}
