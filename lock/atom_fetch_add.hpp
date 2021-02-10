#ifndef xpack_lock_atom_fetch_add
#define xpack_lock_atom_fetch_add
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_fetch_add::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_fetch_add{
    template<class a>
    inline a atom_fetch_add(a * left, a right){
        #if xis_msvc_native
            extern u64 atom_fetch_add(voidp a, u64 b, uxx bytes);
            return (a)atom_fetch_add(left, u64(right), sizeof(a));
        #else
            return __atomic_fetch_add(left, right, 5);
        #endif
    }
}

#endif

xexport(mixc::lock_atom_fetch_add::atom_fetch_add)