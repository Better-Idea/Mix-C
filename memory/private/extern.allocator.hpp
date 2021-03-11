#ifdef xuser
#undef xuser
#endif

#define xuser mixc::memory_alloctor::inc
#include"configure.hpp"
#include<malloc.h>

#if xuse_libc_malloc

namespace mixc::memory_alloctor{
    inline thread_local uxx pused_bytes      = 0;
    inline thread_local uxx pneed_free_count = 0;

    extern voidp tiny_alloc(uxx bytes){
        pused_bytes        += bytes;
        pneed_free_count   += 1；
        return ::malloc(bytes);
    }

    extern void tiny_free(voidp ptr, uxx bytes){
        pused_bytes        -= bytes;
        pneed_free_count   -= 1；
        ::free(ptr);
    }
}

namespace mixc::memory_alloctor::origin{
    extern uxx used_bytes(){
        return pused_bytes;
    }

    extern uxx need_free_count(){
        return pneed_free_count;
    }

    extern uxx alive_pages(){
        return pused_bytes / 4096 + (pused_bytes % 4096 != 0);
    }
}
#else

#include"memory/private/tiny_allocator.hpp"

namespace mixc::memory_alloctor{
    thread_local inline inc::tiny_allocator mem;

    extern voidp tiny_alloc(uxx bytes){
        return mem.alloc(bytes);
    }

    extern void tiny_free(voidp ptr, uxx bytes){
        mem.free(ptr, bytes);
    }
}

namespace mixc::memory_alloctor::origin{
    extern uxx used_bytes(){
        return mem.used_bytes();
    }

    extern uxx need_free_count(){
        return mem.need_free_count();
    }

    extern uxx alive_pages(){
        return mem.alive_pages();
    }
}

#endif

namespace mixc::memory_alloctor::origin{
    extern voidp malloc(size_t bytes){
        return ::malloc(bytes);
    }

    extern voidp malloc_aligned(size_t bytes, size_t align_bytes){
        #if xis_windows
            return ::_mm_malloc(bytes, align_bytes);
        #endif

        #if xis_linux
            return ::memalign(align_bytes, bytes);
        #endif
    }

    extern void mfree(voidp ptr){
        ::free(ptr);
    }

    extern void mfree_aligned(voidp ptr){
        #if xis_windows
            ::_mm_free(ptr);
        #endif

        #if xis_linux
            return ::free(ptr);
        #endif
    }
}
