#ifndef xpack_instruction_shift_left
#define xpack_instruction_shift_left
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_shift_left
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_shift_left{
    inline u128 shift_left(u64 a, uxx b){
        u128 r{};

        if (b < 64){
            r.low   = a << (b);
            r.high  = a >> (64 - b);
        }
        else if (b < 128){
            r.low   = 0;
            r.high  = a << (b - 64);
        }
        return r;
    }
}

#endif

namespace xuser::inc{
    using ::mixc::instruction_shift_left::shift_left;
}
