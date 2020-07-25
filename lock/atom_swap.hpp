#ifndef xpack_lock_atom_swap
#define xpack_lock_atom_swap
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_atom_swap
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_atom_swap{
    extern u64 atom_swap(voidp a, u64 b, uxx bytes);

    template<class a>
    inline a atom_swap(a * left, a right){
        // function equals to 
        // r = left[0]
        // left[0] = right
        #if xis_msvc
            return (a)atom_swap(left, u64(right), sizeof(a));
        #else
            a r;
            __atomic_exchange(left, & right, & r, 5);
            return r;
        #endif
    }
}

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_swap::atom_swap;
}
