#include"configure.hpp"
#include"define/base_type.hpp"

#if xis_msvc
#include<intrin.h>
#include<math.h>

namespace mixc::instruction_time_stamp{
    extern u64 rdtsc(){
        return __rdtsc();
    }
}

#endif

namespace mixc::instruction_mod{
    extern f64 mod(f64 a, f64 b){
        #if xis_x86
            f64 r = 0;
            #if xis_msvc
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