#ifndef xpack_instruction_mod
#define xpack_instruction_mod
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_mod::inc
#include"configure/platform.hpp"
#include"instruction/index_of_last_set.hpp"
#include"math/min.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_mod{
    extern f64 mod(f64 a, f64 b);

    #if xis_msvc_native and xis_os64
    extern u64 mod(u128 a, u64 b);
    #else
    inline u64 mod(u128 a, u64 b){
        #if xis_x86
            #if xis_os64
                #define xhas_invoked
                u64 r = 0;
                asm(""::"a"(a.low),"d"(a.high),"c"(b));
                asm("div %rcx");
                asm("":"=d"(r));
                return r;
            #endif
        #endif

        #ifdef xhas_invoked
            #undef xhas_invoked
        #else
            if (a.high == 0){
                return a.low % b;
            }

            auto shl = [](u128 a, uxx shift){
                u64 shr_low     = (shift == 0 ? 0/*未用到*/ : a.low >> (64 - shift));
                u64 shl_low     = (shift == 0 ? a.low : a.low << (shift));
                a.high          = (a.high << shift) | shr_low;
                a.low           = (shl_low);
                return a;
            };

            for(uxx rest_bits = 64;;){
                uxx i           = inc::index_of_last_set(a.high); 
                i               = 63 - i;
                i               = inc::min(i, rest_bits);
                rest_bits      -= i;
                a               = shl(a, i);

                if (rest_bits == 0){
                    break;
                }
                if (a.high < b){
                    a           = shl(a, 1);
                    rest_bits  -= 1;
                    a.high     -= b;
                }
                else{
                    a.high     %= b;
                }
            }
            auto r              = a.high % b;
            return r;
        #endif
    }
    #endif

}

#endif

xexport(mixc::instruction_mod::mod)