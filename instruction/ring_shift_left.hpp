#ifndef xpack_instruction_ring_shift_left
#define xpack_instruction_ring_shift_left
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_ring_shift_left
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_ring_shift_left{
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
#endif

namespace xuser::inc{
    using ::mixc::instruction_ring_shift_left::ring_shift_left;
}
