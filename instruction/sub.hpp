#ifndef xpack_instruction_sub
#define xpack_instruction_sub
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_sub
#include"configure.hpp"
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_sub{
    // u128.high = a >= b + cf ? 0 : 1
    // u128.low  = a - b - cf
    inline u128 sub(u64 a, u64 b, u64 cf){
        u128 r{};

        #if xis_x86
            #if xis_clang
                #define xcall(bits,...)  __builtin_ia32_subborrow_ ## bits(__VA_ARGS__)
            #else
                #define xcall(bits,...)  __builtin_ia32_sbb_ ## bits(__VA_ARGS__)
            #endif

            #if xis_os64
                r.high   = xcall(u64, u08(cf), a, b, & r.low);
            #elif xis_os32
                u32 cs[2];
                u32 as[] = { u32(a), u32(a >> 32) };
                u32 bs[] = { u32(b), u32(b >> 32) };
                cf       = xcall(u32, u08(cf), as[0], bs[0], & cs[0]);
                cf       = xcall(u32, u08(cf), as[1], bs[1], & cs[1]);
                r.low    = cs[0] | (cs[1] << 32);
                r.high   = cf;
            #endif

            #undef xcall
        #endif
        return r;
    }

    inline u128 sub(u64 a, u64 b){
        return sub(a, b, 0);
    }
}

#endif

namespace xuser::inc{
    using ::mixc::instruction_sub::sub;
}
