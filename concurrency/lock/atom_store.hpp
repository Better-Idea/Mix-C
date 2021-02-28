#ifndef xpack_lock_atom_store
#define xpack_lock_atom_store
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_store::inc
#include"concurrency/lock/atom_swap.hpp"
#include"macro/xexport.hpp"
#include"meta/unsigned_type.hpp"
#include"memop/cast.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_store{
    template<class a0_t, class a1_t = a0_t>
    inline void atom_store(a0_t * left, a1_t right){
        using u0_t  = inc::unsigned_type<a0_t>;
        using u1_t  = inc::unsigned_type<a1_t>;
        using up_t  = u0_t *;
        inc::atom_swap((up_t)left, (u0_t)inc::cast<u1_t>(right));
    }
}

#endif

xexport(mixc::concurrency_lock_atom_store::atom_store)
