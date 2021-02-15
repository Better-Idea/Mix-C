#ifndef xpack_lock_atom_store
#define xpack_lock_atom_store
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_store::inc
#include"concurrency/lock/atom_swap.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_store{
    template<class a>
    inline void atom_store(a * left, a right){
        inc::atom_swap<a>(left, right);
    }
}

#endif

xexport(mixc::concurrency_lock_atom_store::atom_store)