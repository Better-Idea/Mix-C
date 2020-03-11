#pragma once
#include"define/base_type.hpp"

namespace mixc{
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
