#ifndef xpack_lock_atom_xor
#define xpack_lock_atom_xor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_xor::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_xor{
    template<class a>
    inline a atom_xor(a * left, a right){
        #if xis_msvc_compatible
            extern u64 atom_xor(voidp a, u64 b, uxx bytes);
            return (a)atom_xor(left, u64(right), sizeof(a));
        #else
            return __atomic_xor_fetch(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_xor::atom_xor)