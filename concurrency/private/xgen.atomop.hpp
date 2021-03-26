#if defined xa_name && defined xa_func && defined xa_addition
#include"configure/switch.hpp"
#include"define/base_type.hpp"
#include"macro/xlink.hpp"
#include<intrin.h>

#ifdef xa_is_sub
    #undef  xa_is_sub
    #define xa_b        (0 - b)
#else
    #define xa_b        (b)
#endif

namespace mixc::xlink2(concurrency_lock_, xa_name){
    extern u08 atom(u08p a, u08 b){
        return (u08)(xlink2(xa_func, 8)(
            (char *)(a), static_cast<char>(xa_b)
        ) xa_addition);
    }

    extern u16 atom(u16p a, u16 b){
        return (u16)(xlink2(xa_func, 16)(
            (short *)(a), static_cast<short>(xa_b)
        ) xa_addition);
    }

    extern u32 atom(u32p a, u32 b){
        return (u32)(xa_func(
            (long *)(a), static_cast<long>(xa_b)
        ) xa_addition);
    }

    #if xis_os32
    #else
    extern u64 atom(u64p a, u64 b){
        return (u64)(xlink2(xa_func, 64)(
            (__int64 *)(a), static_cast<__int64>(xa_b)
        ) xa_addition);
    }
    #endif
}

#undef xa_name
#undef xa_func
#undef xa_addition
#undef xa_b
#endif
