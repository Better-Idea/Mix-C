#ifndef xpack_instruction_bit_test_and_reverse
#define xpack_instruction_bit_test_and_reverse
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_bit_test_and_reverse::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_bit_test_and_reverse{
    #define xgen(type)                                              \
        inline bool bit_test_and_reverse(type * value, uxx index){  \
            type mask   = (type(1) << index);                       \
            bool result = (value[0] & mask) != 0;                   \
            value[0]   ^= mask;                                     \
            return result;                                          \
        }                                                           \

    xgen(u64);
    xgen(u32);
    xgen(u16);
    xgen(u08);
    #undef xgen
}

#endif

xexport(mixc::instruction_bit_test_and_reverse::bit_test_and_reverse)