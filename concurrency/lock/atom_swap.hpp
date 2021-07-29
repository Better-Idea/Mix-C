#ifndef xpack_lock_atom_swap
#define xpack_lock_atom_swap
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_atom_swap::inc
#include"configure/switch.hpp"
#include"macro/xexport.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_atom_swap{
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u08 atom(u08p a, u08 b);
        extern u16 atom(u16p a, u16 b);
        extern u32 atom(u32p a, u32 b);

        #if xis_os32
        #else
        extern u64 atom(u64p a, u64 b);
        #endif
    #endif
}

namespace mixc::concurrency_lock_atom_swap::origin{
    template<class type_t>
    inline type_t atom_swap(type_t * left, type_t right){
        // function equals to 
        // r = left[0]
        // left[0] = right
        using u0_t  = inc::unsigned_type<type_t>;
        using up_t  = u0_t *;

        #if xis_msvc_native
            u0_t r  = (u0_t)atom(up_t(left), *(u0_t *)(& right));
        #else
            u0_t r;
            __atomic_exchange((up_t)left, (up_t)& right, & r, 5);
        #endif
        return *(type_t *)& r;
    }
}

#endif

xexport_space(mixc::concurrency_lock_atom_swap::origin)
