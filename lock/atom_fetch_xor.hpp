#ifndef xpack_lock_atom_fetch_xor
#define xpack_lock_atom_fetch_xor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_fetch_xor::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_fetch_xor{
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u64 atom_fetch_xor(voidp a, u64 b, uxx bytes);
    #endif

    template<class a>
    inline a atom_fetch_xor(a * left, a right){
        #if xis_msvc_native
            return (a)atom_fetch_xor(left, u64(right), sizeof(a));
        #else
            return __atomic_fetch_xor(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_fetch_xor::atom_fetch_xor)