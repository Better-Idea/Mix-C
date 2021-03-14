#ifndef xpack_lock_atom_swap
#define xpack_lock_atom_swap
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_swap::inc
#include"configure.hpp"
#include"macro/xexport.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_swap{
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u64 atom_swap(voidp a, u64 b, uxx bytes);
    #endif

    template<class type_t>
    inline type_t atom_swap(type_t * left, type_t right){
        // function equals to 
        // r = left[0]
        // left[0] = right
        using u0_t  = inc::unsigned_type<type_t>;
        using up_t  = u0_t *;

        #if xis_msvc_native
            u0_t r  = (u0_t)atom_swap((up_t)(left), *(u0_t *)(& right), sizeof(u0_t));
        #else
            u0_t r;
            __atomic_exchange((up_t)left, (up_t)& right, & r, 5);
        #endif
        return *(type_t *)& r;
    }
}

#endif

xexport(mixc::concurrency_lock_atom_swap::atom_swap)
