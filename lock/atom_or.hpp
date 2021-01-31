#ifndef xpack_lock_atom_or
#define xpack_lock_atom_or
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_or::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_or{
    extern u64 atom_or(voidp a, u64 b, uxx bytes);

    template<class a>
    inline a atom_or(a * left, a right){
        #if xis_msvc_compatible
            return (a)atom_or(left, u64(right), sizeof(a));
        #else
            return __atomic_or_fetch(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_or::atom_or)