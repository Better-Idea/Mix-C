#pragma once
#include"define/base_type.hpp"

namespace mixc{
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
