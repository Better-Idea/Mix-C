#ifndef xpack_instruction_mul
#define xpack_instruction_mul
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_mul::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_mul{
    #if xis_msvc_native
    extern u128 mul(u64 a, u64 b);
    #else
    inline u128 mul(u64 a, u64 b){
        #if xis_x86
            #if xis_os64
                #define xhas_invoked
                u128 r{};
                asm(""::"a"(a),"d"(0), "c"(b));
                asm("mul %rcx");
                asm("":"=a"(r.low),"=d"(r.high));
                return r;
            #endif
        #endif

        #ifdef xhas_invoked
            #undef xhas_invoked
        #else
            u64  ptr_a[]                = { u32(a), u32(a >> 32) };
            u64  ptr_b[]                = { u32(b), u32(b >> 32) };
            u64  product_l[4]           = {};
            u64  product_h[4]           = {};
            u32  r[4]                   = {};

            for(uxx i = 0; i < 2; i++){
                for(uxx j = 0; j < 2; j++){
                    uxx idx             = i + j;
                    u64 t               = u64(ptr_a[i]) * u64(ptr_b[j]);
                    product_l[idx]     += u32(t >> 0 );
                    product_h[idx + 1] += u32(t >> 32);
                }
            }

            for(uxx i = 0, cf = 0/*cf 可能大于 1*/; i < 4; i++){
                u64 sum                 = product_l[i] + product_h[i] + cf;
                r[i]                    = sum;
                cf                      = sum >> 32;
            }

            u128 v;
            v.low                       = (u64(r[1]) << 32) | r[0];
            v.high                      = (u64(r[3]) << 32) | r[2];
            return v;
        #endif
    }
    #endif
}

#endif

xexport(mixc::instruction_mul::mul)