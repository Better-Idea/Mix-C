#ifndef xpack_lock_atom_swap
#define xpack_lock_atom_swap
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_swap::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_swap{
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u64 atom_swap(voidp a, u64 b, uxx bytes);
    #endif

    template<class a>
    inline a atom_swap(a * left, a right){
        // function equals to 
        // r = left[0]
        // left[0] = right
        #if xis_msvc_native
            return (a)atom_swap(left, u64(right), sizeof(a));
        #else
            a r;
            __atomic_exchange(left, & right, & r, 5);
            return r;
        #endif
    }
}

#endif

xexport(mixc::concurrency_lock_atom_swap::atom_swap)