#ifndef xpack_concurrency_lock_cache_exchange
#define xpack_concurrency_lock_cache_exchange
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_cache_exchange::inc
#include"configure/platform.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_cache_exchange::origin{
    #if xis_msvc_native
        extern void cache_exchange();
    #else
        inline void cache_exchange(){
            #if xis_x86
            asm("mfence");
            #else
            #error "TODO"
            #endif
        }
    #endif
    
}

#endif

xexport_space(mixc::concurrency_lock_cache_exchange::origin)