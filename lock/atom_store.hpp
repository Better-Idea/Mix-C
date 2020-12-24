#ifndef xpack_lock_atom_store
#define xpack_lock_atom_store
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_store::inc
#include"lock/atom_swap.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_store{
    template<class a>
    inline void atom_store(a * left, a right){
        inc::atom_swap(left, right);
    }
}

#endif

xexport(mixc::lock_atom_store::atom_store)