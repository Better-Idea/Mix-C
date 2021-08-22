#ifndef xpack_concurrency_lock_cache_store
#define xpack_concurrency_lock_cache_store
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_cache_store::inc
#include"configure/platform.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_cache_store::origin{
    #if xis_msvc_native
        extern void cache_store();
    #else
        inline void cache_store(){
            #if xis_x86
            asm("sfence");
            #else
            #error "TODO"
            #endif
        }
    #endif
    
}

#endif

xexport_space(mixc::concurrency_lock_cache_store::origin)