#include"configure.hpp"
#include"define/base_type.hpp"

#if xis_windows
#include<intrin.h>
namespace mixc::instruction_time_stamp{
    extern u64 rdtsc(){
        return __rdtsc();
    }
}

#endif

#if not xis_msvc
namespace mixc::instruction_mod{
    extern f64 mod(f64 a, f64 b){
        #if xis_x86
            f64 r = 0;
            asm("push %rax");
            asm("fldl %0"::"m"(b));
            asm("fldl %0"::"m"(a));
            asm("L:");
            asm("fprem");
            asm("fstsw %ax");
            asm("sahf");
            asm("jp L");
            asm("fstp %%st(1)":"=t"(r));
            asm("pop %rax");
            return r;
        #endif
    }
}
#endif