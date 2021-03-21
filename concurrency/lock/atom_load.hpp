#ifndef xpack_lock_atom_load
#define xpack_lock_atom_load
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_load::inc
#include"concurrency/lock/atom_fetch_or.hpp"
#include"macro/xexport.hpp"
#include"meta/unsigned_type.hpp"
#include"memop/cast.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_load::origin{
    template<class type_t>
    requires(sizeof(type_t) <= sizeof(uxx))
    inline type_t atom_load(type_t * left){
        using u_t   = inc::unsigned_type<type_t>;
        u_t v       = inc::atom_fetch_or<u_t>((u_t *)left, 0);
        return inc::cast<type_t>(v);
    }
}

#endif

xexport_space(mixc::concurrency_lock_atom_load::origin)
