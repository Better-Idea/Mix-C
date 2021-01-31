#ifndef xpack_lock_atom_add
#define xpack_lock_atom_add
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_add::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_add{
    extern u64 atom_add(voidp a, u64 b, uxx bytes);

    template<class a>
    inline a atom_add(a * left, a right){
        #if xis_msvc_compatible
            return (a)atom_add(left, u64(right), sizeof(a));
        #else
            return __atomic_add_fetch(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_add::atom_add)