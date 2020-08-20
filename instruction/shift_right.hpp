#ifndef xpack_instruction_shift_right
#define xpack_instruction_shift_right
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_shift_right
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_shift_right{
    inline u128 shift_right(u64 a, uxx b){
        u128 r{};

        if (b < 64){
            r.high  = a >> (b);
            r.low   = a << (64 - b);
        }
        else if (b < 128){
            r.high  = 0;
            r.low   = a >> (b - 64);
        }
        return r;
    }
}

#endif

namespace xuser::inc{
    using ::mixc::instruction_shift_right::shift_right;
}
