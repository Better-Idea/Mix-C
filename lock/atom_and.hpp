#ifndef xpack_lock_atom_and
#define xpack_lock_atom_and
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_and
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_and{
    extern u64 atom_and(voidp a, u64 b, uxx bytes);

    template<class a>
    inline a atom_and(a * left, a right){
        #if xis_msvc
            return (a)atom_and(left, u64(right), sizeof(a));
        #else
            return __atomic_and_fetch(left, right, 5);
        #endif
    }
}

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_and::atom_and;
}
