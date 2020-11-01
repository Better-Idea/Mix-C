#ifndef xpack_instruction_mul
#define xpack_instruction_mul
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_mul::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_mul{
    inline u128 mul(u64 a, u64 b){
        #if xis_x86
            #if xis_os64
                u128 r{};
                asm(""::"a"(a),"d"(0), "c"(b));
                asm("mul %rcx");
                asm("":"=a"(r.low),"=d"(r.high));
                return r;
            #endif
        #endif
    }
}

#endif

xexport(mixc::instruction_mul::mul)