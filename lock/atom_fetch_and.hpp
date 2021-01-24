#ifndef xpack_lock_atom_fetch_and
#define xpack_lock_atom_fetch_and
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_fetch_and::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_fetch_and{
    extern u64 atom_fetch_and(voidp a, u64 b, uxx bytes);

    template<class a>
    inline a atom_fetch_and(a * left, a right){
        #if xis_msvc
            return (a)atom_fetch_and(left, u64(right), sizeof(a));
        #else
            return __atomic_fetch_and(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_fetch_and::atom_fetch_and)