#ifndef xpack_lock_atom_xor
#define xpack_lock_atom_xor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_xor::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_xor{
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u64 atom_xor(voidp a, u64 b, uxx bytes);
    #endif

    template<class a>
    inline a atom_xor(a * left, a right){
        #if xis_msvc_native
            return (a)atom_xor(left, u64(right), sizeof(a));
        #else
            return __atomic_xor_fetch(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::concurrency_lock_atom_xor::atom_xor)