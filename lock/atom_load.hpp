#ifndef xpack_lock_atom_load
#define xpack_lock_atom_load
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_load::inc
#include"lock/atom_fetch_or.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_load{
    template<class a>
    inline a atom_load(a * left){
        return inc::atom_fetch_or(left, 0);
    }
}

#endif

xexport(mixc::lock_atom_load::atom_load)