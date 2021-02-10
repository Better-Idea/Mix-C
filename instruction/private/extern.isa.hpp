#include"configure.hpp"
#include"define/base_type.hpp"

#if xis_msvc_native
#include<intrin.h>
#include<math.h>

namespace mixc::instruction_time_stamp{
    extern u64 rdtsc(){
        return __rdtsc();
    }
}

namespace mixc::instruction_mul{
    extern u128 mul(u64 a, u64 b){
        unsigned __int64 high;
        unsigned __int64 low = _umul128((unsigned __int64)a, (unsigned __int64)b, & high);
        return u128{}.l(low).h(high);
    }
}

namespace mixc::instruction_mod{
    extern u64 mod(u128 a, u64 b){
        #if xis_os64
            unsigned __int64 remainder = 0;
            _udiv128(a.high, a.low, b, & remainder);
            return remainder;
        #endif
    }
}

#endif

namespace mixc::instruction_mod{
    extern f64 mod(f64 a, f64 b){
        #if xis_x86
            f64 r = 0;
            #if xis_msvc_native
            r = fmod(a, b);
            #else
            asm("fldl %0"::"m"(b));
            asm("fldl %0"::"m"(a));
            asm("push %rax");
            asm("L:");
            asm("fprem");
            asm("fstsw %ax");
            asm("sahf");
            asm("jp L");
            asm("pop %rax");
            asm("fstp %%st(1)":"=t"(r));
            #endif
            return r;
        #endif
    }
}
