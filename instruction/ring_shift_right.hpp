#ifndef xpack_instruction_ring_shift_right
#define xpack_instruction_ring_shift_right
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_ring_shift_right::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_ring_shift_right{
    #define xgen(type,bits)                                 \
    inline type ring_shift_right(type v, uxx s){            \
        type a = v >> s;                                    \
        type b = v << (64 - s);                             \
        return a | b;                                       \
    }

    xgen(u08, 8)
    xgen(u16, 16)
    xgen(u32, 32)
    xgen(u64, 64)
    #undef xgen
}

#endif

xexport(mixc::instruction_ring_shift_right::ring_shift_right)