#ifndef xpack_instruction_mod
#define xpack_instruction_mod
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_mod
#include"configure.hpp"
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_mod{
    extern f64 mod(f64 a, f64 b);
    inline u64 mod(u128 a, u64 b){
        #if xis_x86
            #if xis_os64
                u64 r = 0;
                asm(""::"a"(a.low),"d"(a.high),"c"(b));
                asm("div %rcx");
                asm("":"=d"(r));
                return r;
            #endif
        #endif
    }
}

#endif

namespace xuser::inc{
    using ::mixc::instruction_mod::mod;
}
