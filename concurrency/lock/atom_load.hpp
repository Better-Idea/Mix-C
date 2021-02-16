#ifndef xpack_lock_atom_load
#define xpack_lock_atom_load
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_load::inc
#include"concurrency/lock/atom_fetch_or.hpp"
#include"meta/unsigned_type.hpp"
#include"memop/cast.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_load{
    template<class a>
    inline a atom_load(a * left){
        using u_t = inc::unsigned_type<a>;
        u_t v = inc::atom_fetch_or<u_t>((u_t *)left, 0);
        return inc::cast<a>(v);
    }
}

#endif

xexport(mixc::concurrency_lock_atom_load::atom_load)