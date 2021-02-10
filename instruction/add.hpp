#ifndef xpack_instruction_add
#define xpack_instruction_add
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_add::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_add{
    inline u128 add(u64 a, u64 b, u64 cf){
        u64 low = a + b + cf;
        return u128{}.l(low).h(low < a or low < b);
    }

    inline u128 add(u64 a, u64 b){
        return add(a, b, 0);
    }
}

#endif

xexport(mixc::instruction_add::add)