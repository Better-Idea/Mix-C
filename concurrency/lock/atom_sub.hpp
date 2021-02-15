#ifndef xpack_lock_atom_sub
#define xpack_lock_atom_sub
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_sub::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_sub{
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u64 atom_sub(voidp a, u64 b, uxx bytes);
    #endif

    template<class a>
    inline a atom_sub(a * left, a right){
        #if xis_msvc_native
            return (a)atom_sub(left, u64(right), sizeof(a));
        #else
            return __atomic_sub_fetch(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_sub::atom_sub)