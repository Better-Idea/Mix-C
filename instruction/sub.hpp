#ifndef xpack_instruction_sub
#define xpack_instruction_sub
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_sub::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_sub{
    inline u128 sub(u64 a, u64 b, u64 cf){
        return u128{}.l(a - b - cf).h(a < b or (a == b and cf));
    }

    inline u128 sub(u64 a, u64 b){
        return sub(a, b, 0);
    }
}

#endif

xexport(mixc::instruction_sub::sub)