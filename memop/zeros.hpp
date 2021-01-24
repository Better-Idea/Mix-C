#ifndef xpack_memop_zeros
#define xpack_memop_zeros
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_zeros::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_zeros{
    inline void zeros(voidp target, uxx bytes) {
        while(bytes--){
            ((u08 *)(target))[bytes] = 0;
        }
    }

    template<class a>
    inline void zeros(a * ptr) {
        zeros(ptr, sizeof(a));
    }
}

#endif

xexport(mixc::memop_zeros::zeros)