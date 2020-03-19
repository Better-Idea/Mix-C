#pragma once
#include"define/base_type.hpp"

namespace mixc{
    #define xgen(type,bits)                                 \
    inline type ring_shift_left(type v, uxx s){             \
        type a = v << s;                                    \
        type b = v >> (64 - s);                             \
        return a | b;                                       \
    }

    xgen(u08, 8)
    xgen(u16, 16)
    xgen(u32, 32)
    xgen(u64, 64)
    #undef xgen
}
