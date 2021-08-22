#ifndef xpack_concurrency_lock_cache_load
#define xpack_concurrency_lock_cache_load
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_cache_load::inc
#include"configure/platform.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_cache_load::origin{
    #if xis_msvc_native
        extern void cache_load();
    #else
        inline void cache_load(){
            #if xis_x86
            asm("lfence");
            #else
            #error "TODO"
            #endif
        }
    #endif
    
}

#endif

xexport_space(mixc::concurrency_lock_cache_load::origin)