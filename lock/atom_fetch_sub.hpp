#ifndef xpack_lock_atom_fetch_sub
#define xpack_lock_atom_fetch_sub
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_fetch_sub::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_fetch_sub{
    extern u64 atom_fetch_sub(voidp a, u64 b, uxx bytes);

    template<class a>
    inline a atom_fetch_sub(a * left, a right){
        #if xis_msvc_compatible
            return (a)atom_fetch_sub(left, u64(right), sizeof(a));
        #else
            return __atomic_fetch_sub(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_fetch_sub::atom_fetch_sub)